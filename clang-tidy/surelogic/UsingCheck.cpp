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
  std::cerr << "start\n";
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
  // std::cerr << "found using for " << nsTryingToUse << "\n";

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
    // generate a FixIt
    // because we do not know where it could be included.
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
    // will
    // fix all the uses.
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
  // std::cerr << " ** replace " << sourceText << " with " << newText << "\n";
  if (namespaceToDiag.count(actualNS)) {
    std::pair<clang::SourceRange, std::string> u1{Range, newText};
    namespaceToDiag[actualNS]->use.push_back(u1);
  }
}

/// Handle types in variable declarations
///   string s;
///  becomes
///    std::string s;
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
  clang::QualType QT = VD->getType();

  const clang::Type *UT = QT.getTypePtr();
  if (!UT)
    return;

  if (UT->isBuiltinType())
    return;
  if (UT->getTypeClass() == clang::Type::TypeClass::Typedef) {
    const clang::TypedefType *tdty = UT->getAs<clang::TypedefType>();
    const clang::TypedefNameDecl *tdnd = tdty->getDecl();
    clang::SourceRange range55 = tdnd->getSourceRange();
    CharSourceRange CSR = Lexer::makeFileCharRange(
        CharSourceRange::getTokenRange(range55), *Result.SourceManager,
        Result.Context->getLangOpts());
    std::string typedef55 = Lexer::getSourceText(CSR, *Result.SourceManager,
                                                 Result.Context->getLangOpts())
                                .str();
    std::cerr << " ** THE TYPEDEF : " << typedef55 << "\n";
    std::cerr << " -=- getNameAsString() = " << tdnd->getNameAsString() << "\n";
    std::cout << "-=- getQualifiedNameAsString() = "
              << tdty->getDecl()->getQualifiedNameAsString() << " "
              << VD->getNameAsString() << "\n";
  }

  std::cerr << " ** QT.getAsString() =  " << QT.getAsString() << "\n";
  std::cerr << " ** QT.getCanonicalType().getAsString() =  "
            << QT.getCanonicalType().getAsString() << "\n";
  std::cerr << " ** QT.getUnqualifiedType().getAsString() =  "
            << QT.getUnqualifiedType().getAsString() << "\n";

  clang::QualType ds = UT->getLocallyUnqualifiedSingleStepDesugaredType();

  std::cerr << " ** ds.getAsString() =  " << ds.getAsString() << "\n";

  const clang::IdentifierInfo *idinfo = QT.getBaseTypeIdentifier();
  if (idinfo)
    std::cerr << " ** QT.getBaseTypeIdentifier() = " << idinfo->getName().str()
              << "\n";

  clang::SourceRange RangeForString{Range};
  CharSourceRange CSR = Lexer::makeFileCharRange(
      CharSourceRange::getTokenRange(RangeForString), *Result.SourceManager,
      Result.Context->getLangOpts());
  std::string possibleSemi = Lexer::getSourceText(CSR, *Result.SourceManager,
                                                  Result.Context->getLangOpts())
                                 .str();
  std::cerr << " ** VarDecl: " << possibleSemi << "\n";
}

} // namespace readability
} // namespace surelogic
} // namespace tidy
} // namespace clang
