#include <iostream>
#include "sl/Common/SLUtil.h"
#include "sl/Tooling/CSureASTConsumer.h"
#include "sl/Analysis/ExampleVisitor.h"
#include "sl/Analysis/StartThreadAnalysis.h"

namespace sl {

void CSureASTConsumer::HandleTranslationUnit(clang::ASTContext &Ctx) {
  // This is where our infrastructure for CSure begins
  ExampleVisitor visitor{Ctx};
  StartThreadAnalysis analysis{Ctx};
  // visitor.TraverseDecl(Ctx.getTranslationUnitDecl());
  sl::l() << "\nStarting analysis.\n";
  analysis.TraverseDecl(Ctx.getTranslationUnitDecl());
}

}
