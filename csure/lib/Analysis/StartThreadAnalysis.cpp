#include <cstring>
#include <memory>
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

#include "sl/Analysis/StartThreadAnalysis.h"
#include "sl/Common/SLUtil.h"

namespace sl {

bool StartThreadAnalysis::isInLocalFile(clang::SourceRange range) {
  clang::FileID file =
      astContext.getSourceManager().getFileID(range.getBegin());
  const clang::FileEntry *entry =
      astContext.getSourceManager().getFileEntryForID(file);
  if (entry == NULL) {
    // l() << "** Unable to get file entry for " <<
    // range.getBegin().printToString(astContext.getSourceManager()) << "\n";

    return false;
  }
  const char *name = entry->getName();
  // l() << "   name = " << name << "\n";
  if (name == NULL) {
    return false;
  }
  int n = std::strlen(name);
  return (n < 3 ||
          (name[n - 1] == 'c' && name[n - 2] == 'c' && name[n - 3] == '.'));
  // return (name[0] != '/');
}

bool StartThreadAnalysis::declaresStartsNothing(clang::FunctionDecl *func) {
  for (auto a : func->attrs()) {
    switch (a->getKind()) {
    default:
      break;
    case clang::attr::SureLogicStarts:
      clang::SureLogicStartsAttr *sa =
          func->getAttr<clang::SureLogicStartsAttr>();
      sl::l() << "   - Got @Starts: " << sa->getValue() << "\n";
      return sa->getValue() == "nothing";
    }
  }
  return false;
}

bool StartThreadAnalysis::isThreadType(clang::QualType qt) {
  sl::l() << "Thread? " << qt.getAsString() << "\n";
  const clang::Type *ty = qt.getTypePtrOrNull();

  // for some reason, I find elaborated type wrappers around the type:
  sl::l() << "  isFromAST() = " << ty->isFromAST() << "\n"
          << "  TypeClassName = " << ty->getTypeClassName() << "\n"
          << "  isObjectType() = " << ty->isObjectType() << "\n"
          << "  isBuiltinType() = " << ty->isBuiltinType() << "\n";
  if (const clang::ElaboratedType *ety =
          llvm::dyn_cast<clang::ElaboratedType>(ty)) {
    sl::l() << "removing elaboration...\n";
    return isThreadType(ety->getNamedType());
  } else if (const clang::RecordType *rty =
                 llvm::dyn_cast<clang::RecordType>(ty)) {
    clang::RecordDecl *rd = rty->getDecl();
    clang::DeclContext *dc = rd->getDeclContext();
    bool isStd = false;
    if (clang::NamespaceDecl *ns = llvm::dyn_cast<clang::NamespaceDecl>(dc)) {
      sl::l() << "  in namespace " << ns->getName() << "\n";
      if (ns->getName() == "std")
        isStd = true;
    }
    if (const clang::IdentifierInfo *ii = rd->getIdentifier()) {
      std::string name = ii->getName();
      sl::l() << "  name of record type = " << name << "\n";
      return isStd && name == "thread";
    }
  }
  return false;
}

bool StartThreadAnalysis::VisitAttr(clang::Attr *a) {
  if (isInLocalFile(a->getRange())) {
    l() << " - VisitAttr " << a->getSpelling() << "\n";
  }
  return true;
}

bool StartThreadAnalysis::TraverseFunctionDecl(clang::FunctionDecl *func) {
  if (isInLocalFile(func->getSourceRange())) {
    bool savedDeclaredStarts = contextStartsNothing;
    contextStartsNothing = declaresStartsNothing(func);
    l() << "TraverseFunctionDecl " << func->getNameAsString()
        << " @Starts nothing? " << contextStartsNothing << "\n";
    bool res =
        clang::RecursiveASTVisitor<StartThreadAnalysis>::TraverseFunctionDecl(
            func);
    contextStartsNothing = savedDeclaredStarts;
    return res;
  } else {
    return clang::RecursiveASTVisitor<
        StartThreadAnalysis>::TraverseFunctionDecl(func);
  }
}

bool StartThreadAnalysis::VisitFunctionDecl(clang::FunctionDecl *func) {
  std::string funcName = func->getNameInfo().getName().getAsString();
  clang::SourceRange range = func->getSourceRange();

  if (isInLocalFile(range)) {
    /*
    l() << "** " << funcName << ": "
    << range.getBegin().printToString(rewriter.getSourceMgr())
    << ", "
    << range.getEnd().printToString(rewriter.getSourceMgr())
    << "\n";

    rewriter.ReplaceText(func->getLocation(), funcName.length(), "add5");
    */
    l() << " - VisitFunctionDecl " << funcName << "\n";

    for (auto a : func->attrs()) {
      sl::l() << "   - Looking at attr on " << funcName << ": "
              << a->getSpelling() << "\n";
      switch (a->getKind()) {
      default:
        break;
      case clang::attr::SureLogicStarts:
        clang::SureLogicStartsAttr *sa =
            func->getAttr<clang::SureLogicStartsAttr>();
        sl::l() << "   - Got @Starts: " << sa->getValue() << "\n";
        break;
      }
    }
  }
  return true;
}

bool StartThreadAnalysis::VisitCallExpr(clang::CallExpr *c) {
  if (isInLocalFile(c->getSourceRange())) {
    sl::l() << " JTB - VisitCallExpr ";
    c->dump(sl::l(), astContext.getSourceManager());
    sl::l() << "\n";

    sl::l() << "   - Callee decl ";
    clang::Decl *d = c->getCalleeDecl();
    if (!d) {
      sl::l() << "<null>";
    } else if (clang::NamedDecl *nd = llvm::dyn_cast<clang::NamedDecl>(d)) {
      l() << nd->getNameAsString();
    } else {
      d->print(l());
    }
    l() << "\n";
    for (auto a : d->attrs()) {
      l() << "     - Attr on callee decl " << a->getSpelling() << "\n";
    }
    if (clang::FunctionDecl *func = llvm::dyn_cast<clang::FunctionDecl>(d)) {
      if (contextStartsNothing && !declaresStartsNothing(func)) {
        l() << "!!! Found unsafe call to " << func->getNameAsString()
            << " that may start a thread\n";
      }
    }
  }
  return true;
}

bool StartThreadAnalysis::VisitVarDecl(clang::VarDecl *decl) {
  if (isInLocalFile(decl->getSourceRange())) {
    if (decl->isLocalVarDecl() && decl->hasLocalStorage()) {
      clang::QualType t = decl->getType();
      sl::l() << "VarDecl(" << decl->getNameAsString() << ") of type "
              << t.getAsString() << "\n";
      if (isThreadType(t) && contextStartsNothing) {
        sl::l() << "!!!! Found thread " << decl->getNameAsString()
                << " in code that is not supposed to have them\n";
      }
    }
  }
  return true;
}

bool StartThreadAnalysis::VisitStmt(clang::Stmt *st) {
  /*
  if (ReturnStmt *ret = dyn_cast<ReturnStmt>(st)) {
  rewriter.ReplaceText(ret->getRetValue()->getLocStart(), 6, "val");
  l() << "** Rewrote ReturnStmt\n";
  }
  if (CallExpr *call = dyn_cast<CallExpr>(st)) {
  rewriter.ReplaceText(call->getLocStart(), 7, "add5");
  l() << "** Rewrote function call\n";
  }
  */
  return true;
}

/*
        virtual bool VisitReturnStmt(ReturnStmt *ret) {
        rewriter.ReplaceText(ret->getRetValue()->getLocStart(), 6, "val");
        l() << "** Rewrote ReturnStmt\n";
        return true;
        }

        virtual bool VisitCallExpr(CallExpr *call) {
        rewriter.ReplaceText(call->getLocStart(), 7, "add5");
        l() << "** Rewrote function call\n";
        return true;
        }
        */

/*
virtual bool VisitRecordDecl(RecordDecl *r) {
l() << "Found record: " << r->getDeclName().getAsString() << "\n";
return true;
}
*/

bool StartThreadAnalysis::VisitCXXRecordDecl(clang::CXXRecordDecl *r) {
  if (isInLocalFile(r->getSourceRange())) {
    l() << " - VisitCXXRecordDecl " << r->getDeclName().getAsString() << "\n";
  }
  return true;
}
}
