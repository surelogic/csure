#include "clang/Lex/Preprocessor.h"

#include "sl/Common/SLUtil.h"
#include "sl/Tooling/CSureASTConsumer.h"
#include "sl/Tooling/CSurePreprocessorConsumer.h"
#include "sl/Tooling/CSureToolAction.h"

namespace sl {

std::unique_ptr<clang::ASTConsumer> CSureToolAction::CreateASTConsumer(
    clang::CompilerInstance &CI, llvm::StringRef file) {
  CI.getPreprocessor().addPPCallbacks(
      std::unique_ptr<clang::PPCallbacks>{new CSurePreprocessorConsumer{}});
  return std::unique_ptr<clang::ASTConsumer>{new CSureASTConsumer{}};
}

bool CSureToolAction::ParseArgs(const clang::CompilerInstance &CI,
                                const std::vector<std::string> &args) {
  // TODO this is not called?
  l() << "o Working dir " << CI.getFileSystemOpts().WorkingDir << "\n";
  l() << "o Output going to " << CI.getFrontendOpts().OutputFile << "\n";
  return true;
}

bool CSureToolAction::BeginSourceFileAction(clang::CompilerInstance &CI,
                                            llvm::StringRef Filename) {
  l() << "Source file " << Filename << "\n";
  return PluginASTAction::BeginSourceFileAction(CI, Filename);
}
}
