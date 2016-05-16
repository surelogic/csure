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
#include <memory>
#include <utility>

#include "sl/Analysis/ExampleVisitor.h"
#include "sl/Common/SLUtil.h"

namespace sl {

bool ExampleVisitor::isInLocalFile(clang::SourceRange range) {
  clang::FileID file =
      astContext.getSourceManager().getFileID(range.getBegin());
  const clang::FileEntry *entry =
      astContext.getSourceManager().getFileEntryForID(file);
  if (entry == NULL) {
    /*
    l() << "** Unable to get file entry for " <<
    range.getBegin().printToString(astContext->getSourceManager()) << "\n";
    */
    return false;
  }
  const char *name = entry->getName();
  if (name == NULL) {
    return false;
  }
  return (name[0] != '/');
}

bool ExampleVisitor::VisitAttr(clang::Attr *a) {
  if (isInLocalFile(a->getRange())) {
    l() << " - VisitAttr " << a->getSpelling() << "\n";
  }
  return true;
}

bool ExampleVisitor::VisitFunctionDecl(clang::FunctionDecl *func) {
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

bool ExampleVisitor::VisitCallExpr(clang::CallExpr *c) {
  sl::l() << " - VisitCallExpr ";
  c->dump(sl::l(), astContext.getSourceManager());
  sl::l() << "\n";

  sl::l() << "   - Callee decl ";
  clang::Decl *d = c->getCalleeDecl();
  d->dump(l());
  l() << "\n";
  for (auto a : d->attrs()) {
    l() << "     - Attr on callee decl " << a->getSpelling() << "\n";
  }
  return true;
}

bool ExampleVisitor::VisitStmt(clang::Stmt *st) {
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

bool ExampleVisitor::VisitCXXRecordDecl(clang::CXXRecordDecl *r) {
  if (isInLocalFile(r->getSourceRange())) {
    l() << " - VisitCXXRecordDecl " << r->getDeclName().getAsString() << "\n";
  }
  return true;
}
}
