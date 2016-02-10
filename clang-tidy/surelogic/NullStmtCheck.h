#ifndef LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_SURELOGIC_NULLSTMTCHECK_H
#define LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_SURELOGIC_NULLSTMTCHECK_H

#include "../ClangTidy.h"

namespace clang {
namespace tidy {
namespace surelogic {
namespace readability {

/// \brief This check uncovers null or empty statments in functions.
///
/// For example the code below would be flagged
///    int f() {
///      ;
///      return 5;
///    }
/// because of the ";" which is a null or empty statement.
///
/// A fix may be applied which removes the ";".
class NullStmtCheck : public ClangTidyCheck {
public:
  NullStmtCheck(StringRef Name, ClangTidyContext *Context)
      : ClangTidyCheck(Name, Context) {}
  void registerMatchers(ast_matchers::MatchFinder *Finder) override;
  void check(const ast_matchers::MatchFinder::MatchResult &Result) override;
};

} // namespace build
} // namespace surelogic
} // namespace tidy
} // namespace clang

#endif // LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_SURELOGIC_NULLSTMTCHECK_H
