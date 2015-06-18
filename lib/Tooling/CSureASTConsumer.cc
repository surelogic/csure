#include "sl/Tooling/CSureASTConsumer.h"
#include "sl/Analysis/ExampleVisitor.h"

namespace sl {

void CSureASTConsumer::HandleTranslationUnit(clang::ASTContext &Ctx) {
  // This is where our infrastructure for CSure begins
  ExampleVisitor visitor{Ctx};
  visitor.TraverseDecl(Ctx.getTranslationUnitDecl());
}
}
