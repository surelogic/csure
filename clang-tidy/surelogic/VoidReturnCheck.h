#ifndef LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_SURELOGIC_VOIDRETURNCHECK_H
#define LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_SURELOGIC_VOIDRETURNCHECK_H

#include "../ClangTidy.h"

namespace clang {
namespace tidy {
namespace surelogic {
namespace readability {

// \brief This check uncovers functions returning the result of a
// void function.
//
// For example the code below would be flagged
//
// void f1() { ... }
// void f2() { return f1(); }
// because of the explicit return of the void result of f1().
//
// A fix may be applied which moves the "return" to be a separate statment.
class VoidReturnCheck : public ClangTidyCheck {
public:
  VoidReturnCheck(StringRef Name, ClangTidyContext *Context)
      : ClangTidyCheck(Name, Context) {}
  void registerMatchers(ast_matchers::MatchFinder *Finder) override;
  void check(const ast_matchers::MatchFinder::MatchResult &Result) override;
};

} // namespace build
} // namespace surelogic
} // namespace tidy
} // namespace clang

#endif // LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_SURELOGIC_VOIDRETURNCHECK_H
