#pragma once

#include <memory>
#include <vector>
#include <string>

#include "clang/AST/ASTConsumer.h"
#include "clang/Frontend/FrontendActions.h"
#include "clang/Frontend/CompilerInstance.h"

namespace sl {

class CSureToolAction : public clang::PluginASTAction {
 protected:
  std::unique_ptr<clang::ASTConsumer> CreateASTConsumer(
      clang::CompilerInstance &CI, llvm::StringRef file) override;

  bool ParseArgs(const clang::CompilerInstance &CI,
                 const std::vector<std::string> &args) override;

  virtual bool BeginSourceFileAction(clang::CompilerInstance &CI,
                                     llvm::StringRef Filename) override;
};
}
