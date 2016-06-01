#include "FixedCommentCheck.h"

#include "clang/Frontend/CompilerInstance.h"
#include "clang/Lex/Preprocessor.h"

namespace clang {
namespace tidy {
namespace surelogic {
namespace readability {

class FixedCommentCheck::FixedCommentHandler : public CommentHandler {
public:
  FixedCommentHandler(FixedCommentCheck &Check,
                      llvm::Optional<std::string> User)
      : Check(Check), User(User ? *User : "unknown"),
        FixedMatch("^// *FIXED *(\\(.*\\))?:?( )?(.*)$") {}

  bool HandleComment(Preprocessor &PP, SourceRange Range) override {
    StringRef Text =
        Lexer::getSourceText(CharSourceRange::getCharRange(Range),
                             PP.getSourceManager(), PP.getLangOpts());

    SmallVector<StringRef, 4> Matches;
    if (!FixedMatch.match(Text, &Matches))
      return false;

    StringRef Username = Matches[1];
    StringRef Comment = Matches[3];

    if (!Username.empty())
      return false;

    std::string NewText = ("// FIXED(" + Twine(User) + "): " + Comment).str();

    Check.diag(Range.getBegin(), "missing username/bug in FIXED")
        << FixItHint::CreateReplacement(CharSourceRange::getCharRange(Range),
                                        NewText);
    return false;
  }

private:
  FixedCommentCheck &Check;
  std::string User;
  llvm::Regex FixedMatch;
};

FixedCommentCheck::FixedCommentCheck(StringRef Name, ClangTidyContext *Context)
    : ClangTidyCheck(Name, Context),
      Handler(llvm::make_unique<FixedCommentHandler>(
          *this, Context->getOptions().User)) {}

void FixedCommentCheck::registerPPCallbacks(CompilerInstance &Compiler) {
  Compiler.getPreprocessor().addCommentHandler(Handler.get());
}

} // namespace readability
} // namespace surelogic
} // namespace tidy
} // namespace clang
