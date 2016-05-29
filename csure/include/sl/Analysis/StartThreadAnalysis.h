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
  explicit StartThreadAnalysis(clang::ASTContext &Ctx) : ast_context_{Ctx} {}

  // Returns true if the passed function declaration is
  // annotated with "[[starts("nothing")]]".
  bool declaresStartsNothing(clang::FunctionDecl *func);

  // Returns true if the passed type is a thread.
  bool isThreadType(clang::QualType);

  // Returns if the passed range is within a ".cc" or ".cpp" source file.
  bool InCppFile(clang::SourceRange range);

  bool VisitAttr(clang::Attr *a);

  bool TraverseFunctionDecl(clang::FunctionDecl *func);

  bool VisitFunctionDecl(clang::FunctionDecl *func);

  bool VisitVarDecl(clang::VarDecl *decl);

  bool VisitCallExpr(clang::CallExpr *c);

  bool VisitStmt(clang::Stmt *st);

  bool VisitCXXRecordDecl(clang::CXXRecordDecl *r);

private:
  // Used for getting additional AST information.
  clang::ASTContext &ast_context_;

  // True while visiting a function declaring starts nothing.
  bool context_starts_nothing_;

  void SetPromiseOn(clang::FunctionDecl *func);
  std::shared_ptr<Drop> GetPromiseOrNullOn(clang::FunctionDecl *func);

  std::map<std::string, std::shared_ptr<Drop>> decl_to_promise_;
};

} // namespace sl

#endif // SL_ANALYSIS_STARTTHREADANALYSIS_H_
