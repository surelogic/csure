#include "EmptyDeclCheck.h"

#include "clang/AST/ASTContext.h"
#include "clang/AST/Decl.h"
#include "clang/ASTMatchers/ASTMatchFinder.h"
#include "clang/ASTMatchers/ASTMatchers.h"
#include "clang/Basic/SourceManager.h"

namespace clang {
namespace tidy {
namespace surelogic {
namespace readability {

void EmptyDeclCheck::registerMatchers(
    clang::ast_matchers::MatchFinder *Finder) {
  if (getLangOpts().CPlusPlus || getLangOpts().C99)
    Finder->addMatcher(clang::ast_matchers::decl().bind("anyDecl"), this);
}

void EmptyDeclCheck::check(
    const clang::ast_matchers::MatchFinder::MatchResult &Result) {
  const EmptyDecl *U = Result.Nodes.getNodeAs<EmptyDecl>("anyDecl");
  // we only care about empty declarations
  if (!U)
    return;
  clang::SourceLocation Loc{U->getLocStart()};
  clang::SourceRange Range{U->getSourceRange()};
  if (!Loc.isValid())
    return;

  diag(Loc, "do not use empty declarations in a translation unit")
      << FixItHint::CreateRemoval(Range);
}

} // namespace readability
} // namespace surelogic
} // namespace tidy
} // namespace clang
