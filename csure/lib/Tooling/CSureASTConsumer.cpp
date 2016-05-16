#include "sl/Tooling/CSureASTConsumer.h"
#include "sl/Analysis/ExampleVisitor.h"
#include "sl/Analysis/StartThreadAnalysis.h"
#include "sl/Common/SLUtil.h"
#include <iostream>

namespace sl {

void CSureASTConsumer::HandleTranslationUnit(clang::ASTContext &Ctx) {
  // This is where our infrastructure for CSure begins
  // ExampleVisitor visitor{Ctx};
  // visitor.TraverseDecl(Ctx.getTranslationUnitDecl());
  StartThreadAnalysis analysis{Ctx};
  analysis.TraverseDecl(Ctx.getTranslationUnitDecl());
}
} // namespace sl
