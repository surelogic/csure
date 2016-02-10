#pragma once

#include <memory>
#include <utility>
#include "clang/Driver/Options.h"
#include "clang/AST/AST.h"
#include "clang/AST/ASTContext.h"
#include "clang/AST/ASTConsumer.h"
#include "clang/AST/Attr.h"
#include "clang/AST/RecursiveASTVisitor.h"
#include "clang/Basic/AttrKinds.h"
#include "clang/Frontend/ASTConsumers.h"
#include "clang/Frontend/FrontendActions.h"
#include "clang/Frontend/CompilerInstance.h"
#include "clang/Frontend/FrontendPluginRegistry.h"
#include "clang/Rewrite/Core/Rewriter.h"
#include "clang/Lex/Preprocessor.h"

namespace sl {

class ExampleVisitor : public clang::RecursiveASTVisitor<ExampleVisitor> {
 private:
  clang::ASTContext &astContext;  // used for getting additional AST info

 public:
  explicit ExampleVisitor(clang::ASTContext &Ctx) : astContext{Ctx} {}

  virtual ~ExampleVisitor() {}

  bool isInLocalFile(clang::SourceRange range);

  virtual bool VisitAttr(clang::Attr *a);

  virtual bool VisitFunctionDecl(clang::FunctionDecl *func);

  virtual bool VisitCallExpr(clang::CallExpr *c);

  virtual bool VisitStmt(clang::Stmt *st);

  virtual bool VisitCXXRecordDecl(clang::CXXRecordDecl *r);
};
}
