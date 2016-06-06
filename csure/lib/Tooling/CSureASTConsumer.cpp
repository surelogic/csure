#include "sl/Tooling/CSureASTConsumer.h"
#include "sl/Analysis/ExampleVisitor.h"
#include "sl/Analysis/StartThreadAnalysis.h"
#include "sl/Common/SLUtil.h"
#include <iostream>

/// @file

namespace sl {

void CSureASTConsumer::HandleTranslationUnit(clang::ASTContext &Ctx) {
  // This is where our infrastructure for CSure begins

  // Uncomment the ExampleVisitor if you want to do testing with it.
  // ExampleVisitor visitor{Ctx};
  // visitor.TraverseDecl(Ctx.getTranslationUnitDecl());

  // Verify [[starts("nothing")]] annotations on functions.
  StartThreadAnalysis analysis{Ctx};
  analysis.TraverseDecl(Ctx.getTranslationUnitDecl());
}

} // namespace sl
