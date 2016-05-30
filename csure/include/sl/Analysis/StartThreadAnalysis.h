#ifndef SL_ANALYSIS_STARTTHREADANALYSIS_H_
#define SL_ANALYSIS_STARTTHREADANALYSIS_H_

#include <map>
#include <memory>
#include <string>
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

#include "sl/Sea/Sea.h"

namespace sl {

// \brief Verifies that a function does not start a new thread of execution.
// For example the below C++ snippet would verify in the CSure tool:
//
// [[starts("nothing")]] void hello() {
//   std::cout << "hello, world!" << std::endl;
// }
//
// Results are reported into the drop-sea truth maintence system for later
// output to the user.
class StartThreadAnalysis final
    : public clang::RecursiveASTVisitor<StartThreadAnalysis> {
public:
  // Constructs an instance of this analysis.
  explicit StartThreadAnalysis(clang::ASTContext &ctx) : ctx_{ctx} {}

  bool TraverseFunctionDecl(clang::FunctionDecl *func);

  bool VisitFunctionDecl(clang::FunctionDecl *func);

  bool VisitCXXRecordDecl(clang::CXXRecordDecl *r);

private:
  // Performs actual analysis of a function declaration.
  void FunctionDeclAnalysis(clang::FunctionDecl *func);

  // Used for getting additional AST information.
  clang::ASTContext &ctx_;

  // Remembers promises on function declartions by function signature.
  std::map<std::string, std::shared_ptr<PromiseDrop>> decl_to_promise_;
};

// \brief This analysis helps the main analysis by examining the contents
// of each function and producing proof structures in drop-sea.
class FunctionAnalysis final
    : public clang::RecursiveASTVisitor<FunctionAnalysis> {
public:
  // Constructs an instance of this analysis.
  explicit FunctionAnalysis(
      clang::ASTContext &ctx,
      std::map<std::string, std::shared_ptr<PromiseDrop>> &decl_to_promise,
      std::shared_ptr<PromiseDrop> promise)
      : ctx_{ctx}, decl_to_promise_{decl_to_promise},
        context_promise_starts_nothing_{promise} {}

  bool VisitVarDecl(clang::VarDecl *decl);

  bool VisitCallExpr(clang::CallExpr *c);

  bool VisitCXXRecordDecl(clang::CXXRecordDecl *r);

private:
  // Used for getting additional AST information.
  clang::ASTContext &ctx_;

  // Remembers promises on function declartions by function signature.
  std::map<std::string, std::shared_ptr<PromiseDrop>> &decl_to_promise_;

  // Saves the promise drop while visiting a function annotated starts nothing.
  const std::shared_ptr<PromiseDrop> context_promise_starts_nothing_;
};

} // namespace sl

#endif // SL_ANALYSIS_STARTTHREADANALYSIS_H_
