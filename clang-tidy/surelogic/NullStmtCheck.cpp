#include "NullStmtCheck.h"
#include "clang/AST/ASTContext.h"
#include "clang/AST/Stmt.h"
#include "clang/ASTMatchers/ASTMatchFinder.h"
#include "clang/ASTMatchers/ASTMatchers.h"
#include "clang/Basic/SourceManager.h"

namespace clang {
namespace tidy {
namespace surelogic {
namespace readability {

void NullStmtCheck::registerMatchers(clang::ast_matchers::MatchFinder *Finder) {
  if (getLangOpts().CPlusPlus || getLangOpts().C99)
    Finder->addMatcher(clang::ast_matchers::nullStmt().bind("nullStmt"), this);
}

void NullStmtCheck::check(
    const clang::ast_matchers::MatchFinder::MatchResult &Result) {
  const NullStmt *U = Result.Nodes.getNodeAs<NullStmt>("nullStmt");
  if (!U)
    return;
  clang::SourceLocation Loc{U->getLocStart()};
  clang::SourceRange Range{U->getSourceRange()};
  if (!Loc.isValid())
    return;

  diag(Loc, "do not use null or empty statments in functions")
      << FixItHint::CreateRemoval(Range);
}

} // namespace readability
} // namespace surelogic
} // namespace tidy
} // namespace clang
