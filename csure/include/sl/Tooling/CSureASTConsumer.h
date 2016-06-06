#ifndef SL_TOOLING_CSUREASTCONSUMER_H_
#define SL_TOOLING_CSUREASTCONSUMER_H_

#include "clang/AST/ASTConsumer.h"
#include "clang/AST/ASTContext.h"

namespace sl {

/// \brief Used to examine each translation unit passed to CSure.
class CSureASTConsumer : public clang::ASTConsumer {
  virtual void HandleTranslationUnit(clang::ASTContext &Ctx) override;
};

} // namespace sl

#endif // SL_TOOLING_CSUREASTCONSUMER_H_
