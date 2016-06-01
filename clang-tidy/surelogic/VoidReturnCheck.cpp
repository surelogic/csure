#include "VoidReturnCheck.h"

#include <string>

#include "clang/AST/ASTContext.h"
#include "clang/AST/Decl.h"
#include "clang/ASTMatchers/ASTMatchFinder.h"
#include "clang/ASTMatchers/ASTMatchers.h"
#include "clang/Basic/SourceManager.h"

namespace clang {
namespace tidy {
namespace surelogic {
namespace readability {

void VoidReturnCheck::registerMatchers(
    clang::ast_matchers::MatchFinder *Finder) {
  if (getLangOpts().CPlusPlus || getLangOpts().C99)
    Finder->addMatcher(clang::ast_matchers::returnStmt().bind("returnStmt"),
                       this);
}

void VoidReturnCheck::check(
    const clang::ast_matchers::MatchFinder::MatchResult &Result) {
  const ReturnStmt *U = Result.Nodes.getNodeAs<ReturnStmt>("returnStmt");
  if (!U)
    return;
  const Expr *E = U->getRetValue();
  if (!E)
    return;
  const std::string return_type = E->getType().getAsString();
  if (return_type == "void") {

    clang::SourceLocation ReturnLoc{U->getReturnLoc()};
    clang::SourceLocation ExLoc{E->getExprLoc()};
    clang::SourceLocation EndLoc{U->getLocEnd()};
    clang::SourceRange ReturnRange{ReturnLoc, ExLoc.getLocWithOffset(-1)};
    if (!ExLoc.isValid())
      return;

    diag(ExLoc, "do not return a void expression from a function")
        << FixItHint::CreateRemoval(ReturnRange)
        << FixItHint::CreateInsertion(EndLoc.getLocWithOffset(1), "; return",
                                      true);
  }
}

} // namespace readability
} // namespace surelogic
} // namespace tidy
} // namespace clang
