#ifndef SL_TOOLING_CSURETOOLACTION_H_
#define SL_TOOLING_CSURETOOLACTION_H_

#include <memory>
#include <string>
#include <vector>

#include "clang/AST/ASTConsumer.h"
#include "clang/Frontend/CompilerInstance.h"
#include "clang/Frontend/FrontendActions.h"

namespace sl {

/// \brief The CSure analysis-based verification tool "main" within
/// the Clang compiler tool interface.
class CSureToolAction : public clang::PluginASTAction {
protected:
  std::unique_ptr<clang::ASTConsumer>
  CreateASTConsumer(clang::CompilerInstance &CI, llvm::StringRef file) override;

  bool ParseArgs(const clang::CompilerInstance &CI,
                 const std::vector<std::string> &args) override;

  virtual bool BeginSourceFileAction(clang::CompilerInstance &CI,
                                     llvm::StringRef Filename) override;
};

} // namespace sl

#endif // SL_TOOLING_CSURETOOLACTION_H_
