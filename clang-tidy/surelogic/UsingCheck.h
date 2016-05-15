#ifndef LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_SURELOGIC_USINGCHECK_H
#define LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_SURELOGIC_USINGCHECK_H

#include <memory>
#include <string>
#include <unordered_map>
#include <utility>

#include "../ClangTidy.h"
#include "HeaderFileExtensionsUtils.h"
#include "clang/Basic/Diagnostic.h"

namespace clang {
namespace tidy {
namespace surelogic {
namespace readability {

struct DiagData {
  DiagData(clang::SourceRange value) : usingStatement(value), use() {}
  clang::SourceRange usingStatement;
  std::vector<std::pair<clang::SourceRange, std::string>> use;
};

/// Finds using namespace directives.
///
/// http://google-styleguide.googlecode.com/svn/trunk/cppguide.xml?showone=Namespaces#Namespaces
///
/// The check implements the following rule of the Google C++ Style Guide:
///
///   You may not use a using-directive to make all names from a namespace
///   available.
///
///   \code
///     // Forbidden -- This pollutes the namespace.
///     using namespace foo;
///   \endcode
///
/// The check provides a fix to the code if the violation does not occur
/// in a header file. In the case of a header violation it is not
/// possible to know other code that includes the header and therefore
/// might depend upon the using directive.
///
/// The check supports these options:
///   - `HeaderFileExtensions`: a comma-separated list of filename extensions of
///     header files (The filename extensions should not contain "." prefix).
///     "h,hh,hpp,hxx" by default.
///     For extension-less header files, using an empty string or leaving an
///     empty string between "," if there are other filename extensions.
class UsingCheck final : public ClangTidyCheck {
public:
  UsingCheck(StringRef Name, ClangTidyContext *Context);
  void storeOptions(ClangTidyOptions::OptionMap &Opts) override;
  void registerMatchers(ast_matchers::MatchFinder *Finder) override;
  void check(const ast_matchers::MatchFinder::MatchResult &Result) override;
  void onStartOfTranslationUnit() override;
  void onEndOfTranslationUnit() override;

private:
  std::string getNamespaceOf(const std::string &qualifiedName);
  bool nameShouldBeIgnored(const std::string &qualifiedName);

  void handleUsingDirectiveDecl(
      const clang::ast_matchers::MatchFinder::MatchResult &Result);
  void handleDeclRefExpr(
      const clang::ast_matchers::MatchFinder::MatchResult &Result);
  void
  handleVarDecl(const clang::ast_matchers::MatchFinder::MatchResult &Result);

  // stores a namespace being removed, e.g., "std", mapping to FixIt information
  std::unordered_map<std::string, std::unique_ptr<DiagData>> namespaceToDiag;

  // helps ensure we do not run on header files
  const std::string RawStringHeaderFileExtensions;
  utils::HeaderFileExtensionsSet HeaderFileExtensions;
};

} // namespace build
} // namespace surelogic
} // namespace tidy
} // namespace clang

#endif // LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_SURELOGIC_USINGCHECK_H
