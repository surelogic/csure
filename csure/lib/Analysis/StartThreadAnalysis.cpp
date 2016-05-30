#include "sl/Analysis/StartThreadAnalysis.h"

#include <cstring>
#include <memory>
#include <sstream>
#include <utility>

#include "clang/AST/AST.h"
#include "clang/AST/ASTConsumer.h"
#include "clang/AST/ASTContext.h"
#include "clang/AST/Attr.h"
#include "clang/AST/RecursiveASTVisitor.h"
#include "clang/Basic/AttrKinds.h"
#include "clang/Driver/Options.h"
#include "clang/Frontend/ASTConsumers.h"
#include "clang/Frontend/CompilerInstance.h"
#include "clang/Frontend/FrontendActions.h"
#include "clang/Frontend/FrontendPluginRegistry.h"
#include "clang/Lex/Preprocessor.h"
#include "clang/Rewrite/Core/Rewriter.h"

// Uncomment the next line to get detailed diagnostic output from the analysis.
#define SL_DEBUG
#include "sl/Common/SLDebug.h"
#include "sl/Common/SLUtil.h"
#include "sl/Sea/Sea.h"

namespace sl {

namespace {

///////////////////////////////////
// HELPER FUNCTIONS FOR ANALYSIS //
///////////////////////////////////

const char *ID = "[start thread analysis] ";

// Returns if the passed range is within a ".cc" or ".cpp" source file.
bool InCppFile(clang::SourceRange range, clang::ASTContext &ctx) {
  clang::FileID file = ctx.getSourceManager().getFileID(range.getBegin());
  const clang::FileEntry *entry =
      ctx.getSourceManager().getFileEntryForID(file);
  if (entry == NULL)
    return false;

  const char *name = entry->getName();
  if (name == NULL)
    return false;

  int n = std::strlen(name);
  // Return true if the name ends in ".cpp" or ".cc".
  return (n < 4 || (name[n - 1] == 'p' && name[n - 2] == 'p' &&
                    name[n - 3] == 'c' && name[n - 4] == '.') ||
          (name[n - 1] == 'c' && name[n - 2] == 'c' && name[n - 3] == '.'));
}

// Returns user readable file location information for a source location.
// For example:
// TODO
std::string GetLocationInfo(clang::SourceLocation src_loc,
                            clang::ASTContext &ctx) {
  std::ostringstream convert;
  clang::PresumedLoc pl = ctx.getSourceManager().getPresumedLoc(src_loc);
  convert << pl.getFilename() << ":" << pl.getLine() << ":" << pl.getColumn();
  return convert.str();
}

// Creates a signature to remember to save a promise on a function declaration.
// Used as a utility by StartThreadAnalysis::SetPromiseOn and
// StartThreadAnalysis::GetPromiseOrNullOn which are declared below.
std::string GetSignature(clang::FunctionDecl *func) {
  std::ostringstream sig;
  sig << func->getQualifiedNameAsString() << "(";
  bool first = true;
  for (const clang::ParmVarDecl *param : func->params()) {
    if (first)
      first = false;
    else
      sig << ",";
    sig << param->getOriginalType().getAsString();
  }
  sig << ")->" << func->getReturnType().getAsString();
  return sig.str();
}

// Returns a [[starts("nothing")]] promise from the passed map if one is
// annotated on the passed function. If SetPromiseOn() has not been called
// for the passed then nullptr is returned.
std::shared_ptr<PromiseDrop> GetPromiseOrNullOn(
    clang::FunctionDecl *func,
    std::map<std::string, std::shared_ptr<PromiseDrop>> &decl_to_promise) {
  D(l() << ID << "GetPromiseOrNullOn()\n";)
  return decl_to_promise[GetSignature(func)];
}

// Saves a [[starts("nothing")]] promise in the passed map if one is annotated
// on the passed function.
void SetPromiseOn(
    clang::FunctionDecl *func, clang::ASTContext &ctx,
    std::map<std::string, std::shared_ptr<PromiseDrop>> &decl_to_promise) {
  D(l() << ID << "SetPromiseOn(" << GetSignature(func) << ")\n";)
  if (GetPromiseOrNullOn(func, decl_to_promise) == nullptr) {
    std::shared_ptr<PromiseDrop> promise = Sea::Default()->NewStartsPromise();
    std::string src_loc =
        GetLocationInfo(func->getSourceRange().getBegin(), ctx);
    promise->SetMessage("[[starts(\"nothing\")]] on " +
                        func->getQualifiedNameAsString() + " @ " + src_loc);
    decl_to_promise[GetSignature(func)] = promise;
    D(l() << ID << "-created promise\n";)
  } else {
    D(l() << ID << "-promise already existed\n";)
  }
}

// Returns true if the passed function declaration is annotated
// with "[[starts("nothing")]]". This method also creates a drop-sea
// promise that may be retrieved with a call to GetPromiseOrNullOn(func).
bool IsAnnotatedWithStartsNothing(
    clang::FunctionDecl *func, clang::ASTContext &ctx,
    std::map<std::string, std::shared_ptr<PromiseDrop>> &decl_to_promise) {
  D(l() << ID << "IsAnnotatedWithStartsNothing(" << GetSignature(func)
        << ")\n";)
  for (auto a : func->attrs()) {
    switch (a->getKind()) {
    default:
      break;
    case clang::attr::SureLogicStarts:
      clang::SureLogicStartsAttr *sa =
          func->getAttr<clang::SureLogicStartsAttr>();
      if (sa->getValue() == "nothing") {
        // create and save a promise associated with this function.
        SetPromiseOn(func, ctx, decl_to_promise);
        D(l() << ID << "-annotated [[starts(\"nothing\")]]\n";)
        return true;
      } else {
        // Output a warning about an unsupported starts annoation.
        clang::DiagnosticsEngine &Diags = ctx.getDiagnostics();
        unsigned diagID = Diags.getCustomDiagID(
            clang::DiagnosticsEngine::Warning,
            "Only [[starts(\"nothing\")]] supported (unknown value)");
        Diags.Report(sa->getLocation(), diagID);
      }
    }
  }
  D(l() << ID << "-is not annotated [[starts(\"nothing\")]]\n";)
  return false;
}

// Returns true if the passed type is a thread.
bool IsThreadType(clang::QualType qt) {
  const clang::Type *ty = qt.getTypePtrOrNull();
  bool result = false;

  // We get an elaboraged type in some cases.
  if (const clang::ElaboratedType *ety =
          llvm::dyn_cast<clang::ElaboratedType>(ty)) {
    result = IsThreadType(ety->getNamedType());
  } else if (const clang::RecordType *rty =
                 llvm::dyn_cast<clang::RecordType>(ty)) {
    D(l() << ID << "IsThreadType(" << qt.getAsString() << ") ";)
    clang::RecordDecl *rd = rty->getDecl();
    clang::DeclContext *dc = rd->getDeclContext();
    bool isStd = false;
    if (clang::NamespaceDecl *ns = llvm::dyn_cast<clang::NamespaceDecl>(dc)) {
      D(l() << "[in namespace " << ns->getName() << "] ";)
      if (ns->getName() == "std")
        isStd = true;
    }
    if (const clang::IdentifierInfo *ii = rd->getIdentifier()) {
      std::string name = ii->getName();
      D(l() << "[name of type = " << name << "] ");
      result = isStd && name == "thread";
    }
    D(l() << (result ? "true" : "false") << "\n";)
  }
  return result;
}

} // namespace

///////////////////////////////////////////////////
// MAIN ANALYSIS TO VERIFY [[starts("nothing")]] //
///////////////////////////////////////////////////

void StartThreadAnalysis::FunctionDeclAnalysis(clang::FunctionDecl *func) {
  if (InCppFile(func->getSourceRange(), ctx_)) {
    D(l() << ID << ">>>>>>>>>> " << GetSignature(func) << "\n";)
    if (IsAnnotatedWithStartsNothing(func, ctx_, decl_to_promise_)) {
      D(l() << ID << "-function " << GetSignature(func)
            << " annotated [[starts(\"nothing\")]]\n";)
      // Should never return nullptr within this block
      std::shared_ptr<PromiseDrop> promise =
          GetPromiseOrNullOn(func, decl_to_promise_);

      // Examine the function carefully and setup up proof structure.
      FunctionAnalysis analysis{ctx_, decl_to_promise_, promise};
      analysis.TraverseDecl(func);
    }
    D(l() << ID << "<<<<<<<<<< " << GetSignature(func) << "\n";)
  }
}

bool StartThreadAnalysis::TraverseFunctionDecl(clang::FunctionDecl *func) {
  if (InCppFile(func->getSourceRange(), ctx_)) {
    D(l() << ID << "StartThreadAnalysis::TraverseFunctionDecl("
          << GetSignature(func) << ")\n";)
    FunctionDeclAnalysis(func);
  }
  return true;
}

bool StartThreadAnalysis::VisitFunctionDecl(clang::FunctionDecl *func) {
  if (InCppFile(func->getSourceRange(), ctx_)) {
    D(l() << ID << "StartThreadAnalysis::VisitFunctionDecl("
          << GetSignature(func) << ")\n";)
    FunctionDeclAnalysis(func);
  }
  return true;
}

bool StartThreadAnalysis::VisitCXXRecordDecl(clang::CXXRecordDecl *r) {
  if (InCppFile(r->getSourceRange(), ctx_)) {
    sl::l() << " - VisitCXXRecordDecl " << r->getDeclName().getAsString()
            << "\n";
  }
  return true;
}

///////////////////////////////
// PER-FUNCTION SUB ANALYSIS //
///////////////////////////////

bool FunctionAnalysis::VisitCallExpr(clang::CallExpr *c) {
  if (InCppFile(c->getSourceRange(), ctx_)) {
    clang::Decl *d = c->getCalleeDecl();
    if (clang::FunctionDecl *func = llvm::dyn_cast<clang::FunctionDecl>(d)) {
      D(l() << ID << "FunctionAnalysis::VisitCallExpr("
            << func->getQualifiedNameAsString() << ")\n";)
      if (IsAnnotatedWithStartsNothing(func, ctx_, decl_to_promise_)) {
        // Report a consistent result and link the promise on function being
        // examined as a dependent on the consistency of the function being
        // called.
        std::shared_ptr<ResultDrop> result =
            Sea::Default()->NewConsistentResult();
        std::shared_ptr<PromiseDrop> promise_on_call =
            GetPromiseOrNullOn(func, decl_to_promise_);
        std::ostringstream msg;
        msg << "Call to function '" << GetSignature(func)
            << "()' promises to start no threads @ "
            << GetLocationInfo(c->getRParenLoc(), ctx_);
        D(l() << ID << "-" << msg.str() << "\n");
        result->SetMessage(msg.str());
        result->AddChecked(context_promise_starts_nothing_);
        result->AddTrusted(promise_on_call);
      } else {
        // Report an inconsistent result because the function being called
        // does not promise to start no threads.
        std::shared_ptr<ResultDrop> result =
            Sea::Default()->NewInconsistentResult();
        std::ostringstream msg;
        msg << "Unsafe call to function '" << GetSignature(func)
            << "()' that may start a thread @ "
            << GetLocationInfo(c->getRParenLoc(), ctx_);
        D(l() << ID << "-" << msg.str() << "\n");
        result->SetMessage(msg.str());
        result->AddChecked(context_promise_starts_nothing_);
      }
    }
  }
  return true;
}

bool FunctionAnalysis::VisitVarDecl(clang::VarDecl *decl) {
  if (InCppFile(decl->getSourceRange(), ctx_)) {
    D(l() << ID << "FunctionAnalysis::VisitVarDecl("
          << decl->getQualifiedNameAsString() << ")\n";)
    if (decl->isLocalVarDecl() && decl->hasLocalStorage()) {
      clang::QualType qual_type = decl->getType();
      if (IsThreadType(qual_type)) {
        // Found the starting of a thread within a function that
        // promises not to start any threads, therfore, we report
        // an inconsistency.
        std::shared_ptr<ResultDrop> result =
            Sea::Default()->NewInconsistentResult();
        std::ostringstream msg;
        msg << "Unsafe starting a thread '" << decl->getQualifiedNameAsString()
            << "' @ " << GetLocationInfo(decl->getLocStart(), ctx_);
        D(l() << ID << "-" << msg.str() << "\n");
        result->SetMessage(msg.str());
        result->AddChecked(context_promise_starts_nothing_);
      }
    }
  }
  return true;
}

bool FunctionAnalysis::VisitCXXRecordDecl(clang::CXXRecordDecl *r) {
  // Do not descend into another type declaration.
  return false;
}

} // namespace sl
