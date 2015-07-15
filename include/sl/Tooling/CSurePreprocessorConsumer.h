#pragma once

#include "clang/Frontend/FrontendActions.h"

namespace sl {

class CSurePreprocessorConsumer : public clang::PPCallbacks {
  ~CSurePreprocessorConsumer() {}

  virtual void InclusionDirective(clang::SourceLocation HashLoc, const clang::Token &IncludeTok,
                                  llvm::StringRef FileName, bool IsAngled,
                                  clang::CharSourceRange FilenameRange,
                                  const clang::FileEntry *File, llvm::StringRef SearchPath,
                                  llvm::StringRef RelativePath,
                                  const clang::Module *Imported) override;
};
}
