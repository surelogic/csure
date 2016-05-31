#include "UsingCheck.h"

#include "clang/AST/ASTContext.h"
#include "clang/AST/Decl.h"
#include "clang/AST/DeclBase.h"
#include "clang/AST/Expr.h"
#include "clang/AST/Type.h"
#include "clang/ASTMatchers/ASTMatchFinder.h"
#include "clang/ASTMatchers/ASTMatchers.h"
#include "clang/Basic/IdentifierTable.h"
#include "clang/Basic/SourceLocation.h"
#include "clang/Basic/SourceManager.h"
#include "clang/Lex/Lexer.h"

#include <iostream>
#include <string>

// Uncomment the next line to get detailed diagnostic output from the analysis.
//#define SL_DEBUG
#include "SLDebug.h"

namespace clang {
namespace tidy {
namespace surelogic {
namespace readability {

UsingCheck::UsingCheck(StringRef Name, ClangTidyContext *Context)
    : ClangTidyCheck(Name, Context),
      RawStringHeaderFileExtensions("h,hh,hpp,hxx") {
  if (!utils::parseHeaderFileExtensions(RawStringHeaderFileExtensions,
                                        HeaderFileExtensions, ',')) {
    llvm::errs() << "Invalid header file extension: "
                 << RawStringHeaderFileExtensions << "\n";
  }
}

void UsingCheck::storeOptions(ClangTidyOptions::OptionMap &Opts) {
  Options.store(Opts, "HeaderFileExtensions", RawStringHeaderFileExtensions);
}

void UsingCheck::registerMatchers(clang::ast_matchers::MatchFinder *Finder) {
  // Only register the matchers for C++; the functionality currently does not
  // provide any benefit to other languages, despite being benign.
  if (getLangOpts().CPlusPlus) {
    Finder->addMatcher(clang::ast_matchers::usingDirectiveDecl().bind("using"),
                       this);
    // Finder->addMatcher(clang::ast_matchers::expr().bind("expr"), this);
    Finder->addMatcher(clang::ast_matchers::varDecl().bind("varDecl"), this);
    Finder->addMatcher(clang::ast_matchers::declRefExpr().bind("declRefExpr"),
                       this);
  }
}

std::string UsingCheck::getNamespaceOf(const std::string &qualifiedName) {
  auto pos = qualifiedName.rfind("::");
  if (pos == std::string::npos)
    return "";
  else
    return qualifiedName.substr(0, pos);
}

bool UsingCheck::nameShouldBeIgnored(const std::string &qualifiedName) {
  return qualifiedName.find("operator") != std::string::npos;
}

void UsingCheck::check(
    const clang::ast_matchers::MatchFinder::MatchResult &Result) {
  handleUsingDirectiveDecl(Result);
  handleDeclRefExpr(Result);
  handleVarDecl(Result);
}

void UsingCheck::onStartOfTranslationUnit() {
  ClangTidyCheck::onStartOfTranslationUnit();
}

void UsingCheck::onEndOfTranslationUnit() {
  ClangTidyCheck::onEndOfTranslationUnit();
  // Output all 'using namespace's found and our FixIts
  for (auto it = namespaceToDiag.begin(); it != namespaceToDiag.end(); ++it) {
    clang::DiagnosticBuilder issue =
        diag(it->second->usingStatement.getBegin(),
             "do not use namespace using-directives; use using-declarations "
             "instead");
    issue.AddFixItHint(FixItHint::CreateRemoval(it->second->usingStatement));
    for (auto vit = it->second->use.begin(); vit != it->second->use.end();
         ++vit) {
      issue.AddFixItHint(FixItHint::CreateReplacement(vit->first, vit->second));
    }
  }
}

void UsingCheck::handleUsingDirectiveDecl(
    const clang::ast_matchers::MatchFinder::MatchResult &Result) {
  const UsingDirectiveDecl *U =
      Result.Nodes.getNodeAs<UsingDirectiveDecl>("using");
  if (!U)
    return;

  clang::SourceLocation Loc{U->getLocStart()};
  if (U->isImplicit() || !Loc.isValid())
    return;

  std::string nsTryingToUse{
      U->getNominatedNamespaceAsWritten()->getQualifiedNameAsString()};

  clang::SourceRange Range{U->getSourceRange()};

  // There is a problem with the returned range -- it misses the ";".
  // For example, for "using namespace std;" we don't get the trailing ";".
  // So the code below searches for a semicolon starting at the last token
  // of the node and expands the sourcerange so that the entire statement
  // may be removed. We should always find a semicolon.
  int SemiIndex = 1;
  bool foundSemi = false;
  while (!foundSemi) {
    clang::SourceLocation PastEndLoc{
        U->getLocEnd().getLocWithOffset(SemiIndex)};
    clang::SourceRange RangeForString{PastEndLoc};
    CharSourceRange CSR = Lexer::makeFileCharRange(
        CharSourceRange::getTokenRange(RangeForString), *Result.SourceManager,
        Result.Context->getLangOpts());
    std::string possibleSemi =
        Lexer::getSourceText(CSR, *Result.SourceManager,
                             Result.Context->getLangOpts())
            .str();
    if (possibleSemi == ";") {
      // Found a ";" so expand the range for our fixit below.
      Range.setEnd(PastEndLoc);
      foundSemi = true;
    } else {
      SemiIndex++;
    }
  }
  if (utils::isPresumedLocInHeaderFile(Loc, *Result.SourceManager,
                                       HeaderFileExtensions)) {
    // using directive is in a header file...we report this and do not
    // generate a FixIt because we do not know where it could be
    // included.
    diag(Range.getBegin(),
         "do not use namespace using-directives; use using-declarations "
         "instead (in header file)");
  } else

      if (namespaceToDiag.count(nsTryingToUse) == 0) {
    std::unique_ptr<DiagData> dd(new DiagData(Range));
    namespaceToDiag[nsTryingToUse] = std::move(dd);
  } else {
    // duplicate using directive in the translation unit...we report this and
    // generate a FixIt to remove the directive. The first occurance found
    // will fix all the uses.
    diag(Range.getBegin(),
         "do not use namespace using-directives; use using-declarations "
         "instead")
        << FixItHint::CreateRemoval(Range);
  }
}

void UsingCheck::handleDeclRefExpr(
    const clang::ast_matchers::MatchFinder::MatchResult &Result) {
  /*
   * Handle all method calls and variable references
   */
  const DeclRefExpr *CE = Result.Nodes.getNodeAs<DeclRefExpr>("declRefExpr");
  if (!CE)
    return;

  clang::SourceLocation Loc{CE->getLocStart()};
  // ignore if not in source or in a header file
  if (!Loc.isValid() || utils::isPresumedLocInHeaderFile(
                            Loc, *Result.SourceManager, HeaderFileExtensions))
    return;
  // ignore if use is already qualified
  if (CE->hasQualifier())
    return;
  std::string qualifiedName = CE->getDecl()->getQualifiedNameAsString();
  // ignore operators and such
  if (nameShouldBeIgnored(qualifiedName))
    return;
  std::string actualNS = getNamespaceOf(qualifiedName);
  // ignore if not in a namespace ("" for our code)
  if (actualNS == "")
    return;
  clang::SourceRange Range{CE->getSourceRange()};
  clang::SourceRange RangeForString{Range};
  CharSourceRange CSR = Lexer::makeFileCharRange(
      CharSourceRange::getTokenRange(RangeForString), *Result.SourceManager,
      Result.Context->getLangOpts());
  std::string sourceText = Lexer::getSourceText(CSR, *Result.SourceManager,
                                                Result.Context->getLangOpts())
                               .str();
  std::string newText = actualNS + "::" + sourceText;
  if (namespaceToDiag.count(actualNS)) {
    std::pair<clang::SourceRange, std::string> u1{Range, newText};
    namespaceToDiag[actualNS]->use.push_back(u1);
  }
}

// Handle types in variable declarations. For example:
//   string s;
//  becomes
//    std::string s;
// When "useing namespace std;" is removed.
void UsingCheck::handleVarDecl(
    const clang::ast_matchers::MatchFinder::MatchResult &Result) {
  const VarDecl *VD = Result.Nodes.getNodeAs<VarDecl>("varDecl");
  if (!VD)
    return;

  clang::SourceLocation Loc{VD->getLocStart()};
  if (VD->isImplicit() || !Loc.isValid() ||
      utils::isPresumedLocInHeaderFile(Loc, *Result.SourceManager,
                                       HeaderFileExtensions))
    return;
  clang::SourceRange Range{VD->getSourceRange()};
  clang::SourceRange RangeForString{Range};
  clang::QualType QT = VD->getType();
  const clang::Type *UT = QT.getTypePtr();
  if (!UT)
    return;

  if (UT->isBuiltinType())
    return;

  // TODO(hallorant) This is really a best effort. We need a lot more
  // knowledge of how Clang deals with the complex C++ type sytem to
  // make this transformation sound. However, the below code empirically
  // covers a lot of common cases in real C++ code.
  if (UT->getTypeClass() == clang::Type::TypeClass::Typedef) {
    // Handle the common case of a typedef as well as we can.
    const clang::TypedefType *tdty = UT->getAs<clang::TypedefType>();
    const clang::TypedefNameDecl *tdnd = tdty->getDecl();
    std::string actualNS = tdnd->getQualifiedNameAsString();
    D(std::cerr << "[TypeClass::Typedef] "
                   "clang::TypedefNameDecl->getQualifiedNameAsString() = "
                << actualNS << "\n";)
    std::size_t pos = actualNS.find_last_of("::");
    if (pos == std::string::npos)
      return;
    actualNS = actualNS.substr(0, pos - 1);
    // special case for std:: to fix stuff like "std::__cxx11::string"
    // The standard library uses odd namespace aliases.
    std::string std_prefix{"std::_"};
    if (actualNS.compare(0, std_prefix.length(), std_prefix) == 0)
      actualNS = "std";
    D(std::cerr << "[TypeClass::Typedef] namespace " << actualNS << "\n";)

    CharSourceRange CSR = Lexer::makeFileCharRange(
        CharSourceRange::getTokenRange(RangeForString), *Result.SourceManager,
        Result.Context->getLangOpts());
    std::string sourceText = Lexer::getSourceText(CSR, *Result.SourceManager,
                                                  Result.Context->getLangOpts())
                                 .str();
    std::string newText = actualNS + "::" + sourceText;
    if (namespaceToDiag.count(actualNS)) {
      std::pair<clang::SourceRange, std::string> u1{Range, newText};
      namespaceToDiag[actualNS]->use.push_back(u1);
    }
  } else {
    // Make a best-effort on every other kind of type use.
    std::string qt = clang::QualType::getAsString(QT.split());
    D(std::cerr << "[TypeClass::OTHER] type string " << qt << "\n";)
    std::size_t pos = qt.find_last_of("::");
    if (pos == std::string::npos)
      return;
    std::string actualNS = qt.substr(0, pos - 1);
    pos = actualNS.find_last_of(" ");
    if (pos == std::string::npos)
      return;
    actualNS = actualNS.substr(pos + 1);
    D(std::cerr << "[TypeClass::OTHER] namespace " << actualNS << "\n";)

    CharSourceRange CSR = Lexer::makeFileCharRange(
        CharSourceRange::getTokenRange(RangeForString), *Result.SourceManager,
        Result.Context->getLangOpts());
    std::string sourceText = Lexer::getSourceText(CSR, *Result.SourceManager,
                                                  Result.Context->getLangOpts())
                                 .str();
    std::string newText = actualNS + "::" + sourceText;
    if (namespaceToDiag.count(actualNS)) {
      std::pair<clang::SourceRange, std::string> u1{Range, newText};
      namespaceToDiag[actualNS]->use.push_back(u1);
    }
  }
}

} // namespace readability
} // namespace surelogic
} // namespace tidy
} // namespace clang
