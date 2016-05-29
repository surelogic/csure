#ifndef SL_ANALYSIS_EXAMPLEVISITOR_H_
#define SL_ANALYSIS_EXAMPLEVISITOR_H_

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

// \brief An example analysis visitor used for testing CSure analyses.
class ExampleVisitor : public clang::RecursiveASTVisitor<ExampleVisitor> {
private:
  // Used for getting additional AST information.
  clang::ASTContext &ast_context_;

public:
  explicit ExampleVisitor(clang::ASTContext &Ctx) : ast_context_{Ctx} {}

  virtual ~ExampleVisitor() {}

  bool isInLocalFile(clang::SourceRange range);

  virtual bool VisitAttr(clang::Attr *a);

  virtual bool VisitFunctionDecl(clang::FunctionDecl *func);

  virtual bool VisitCallExpr(clang::CallExpr *c);

  virtual bool VisitStmt(clang::Stmt *st);

  virtual bool VisitCXXRecordDecl(clang::CXXRecordDecl *r);
};

} // namespace sl

#endif // SL_ANALYSIS_EXAMPLEVISITOR_H_
