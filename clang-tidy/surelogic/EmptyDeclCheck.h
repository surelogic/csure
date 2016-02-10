#ifndef LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_SURELOGIC_EMPTYDECLCHECK_H
#define LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_SURELOGIC_EMPTYDECLCHECK_H

#include "../ClangTidy.h"

namespace clang {
namespace tidy {
namespace surelogic {
namespace readability {

/// \brief This check uncovers empty declarations in translation units.
///
/// For example the code below would be flagged
///    #include <string>
///
///    ;
///
///    int f() {
///      return 5;
///    }
/// because of the ";" which is an empty declaration.
///
/// A fix may be applied which removes the ";".
class EmptyDeclCheck : public ClangTidyCheck {
public:
  EmptyDeclCheck(StringRef Name, ClangTidyContext *Context)
      : ClangTidyCheck(Name, Context) {}
  void registerMatchers(ast_matchers::MatchFinder *Finder) override;
  void check(const ast_matchers::MatchFinder::MatchResult &Result) override;
};

} // namespace build
} // namespace surelogic
} // namespace tidy
} // namespace clang

#endif // LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_SURELOGIC_EMPTYDECLCHECK_H
