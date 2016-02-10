#ifndef LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_SURELOGIC_FIXEDCOMMENTCHECK_H
#define LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_SURELOGIC_FIXEDCOMMENTCHECK_H

#include "../ClangTidy.h"

namespace clang {
namespace tidy {
namespace surelogic {
namespace readability {

/// \brief Finds FIXED comments without a username or bug number.
class FixedCommentCheck : public ClangTidyCheck {
public:
  FixedCommentCheck(StringRef Name, ClangTidyContext *Context);
  void registerPPCallbacks(CompilerInstance &Compiler) override;

private:
  class FixedCommentHandler;
  std::unique_ptr<FixedCommentHandler> Handler;
};

} // namespace readability
} // namespace surelogic
} // namespace tidy
} // namespace clang

#endif // LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_SURELOGIC_FIXEDCOMMENTCHECK_H
