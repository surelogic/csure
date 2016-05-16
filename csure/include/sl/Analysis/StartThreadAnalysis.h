#pragma once

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

namespace sl {

class StartThreadAnalysis
    : public clang::RecursiveASTVisitor<StartThreadAnalysis> {
private:
  clang::ASTContext &astContext; // used for getting additional AST info

  bool contextStartsNothing; // true while visiting a function declaring starts
                             // nothing

public:
  explicit StartThreadAnalysis(clang::ASTContext &Ctx) : astContext{Ctx} {}

  ~StartThreadAnalysis() {}

  bool declaresStartsNothing(clang::FunctionDecl *func);
  bool isThreadType(clang::QualType);

  bool isInLocalFile(clang::SourceRange range);

  bool VisitAttr(clang::Attr *a);

  bool TraverseFunctionDecl(clang::FunctionDecl *func);

  bool VisitFunctionDecl(clang::FunctionDecl *func);

  bool VisitVarDecl(clang::VarDecl *decl);

  bool VisitCallExpr(clang::CallExpr *c);

  bool VisitStmt(clang::Stmt *st);

  bool VisitCXXRecordDecl(clang::CXXRecordDecl *r);
};

// end namespace
}
