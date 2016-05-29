#ifndef SL_TOOLING_CSUREPREPROCESSORCONSUMER_H_
#define SL_TOOLING_CSUREPREPROCESSORCONSUMER_H_

#include "clang/Frontend/FrontendActions.h"

namespace sl {

// \brief Used to examine each "#include" that exists in a translation
// unit and keep track of nested includes. This is not needed by all
// analyses.
class CSurePreprocessorConsumer : public clang::PPCallbacks {
  ~CSurePreprocessorConsumer() {}

  virtual void InclusionDirective(clang::SourceLocation HashLoc,
                                  const clang::Token &IncludeTok,
                                  llvm::StringRef FileName, bool IsAngled,
                                  clang::CharSourceRange FilenameRange,
                                  const clang::FileEntry *File,
                                  llvm::StringRef SearchPath,
                                  llvm::StringRef RelativePath,
                                  const clang::Module *Imported) override;
};

} // namespace sl

#endif // SL_TOOLING_CSUREPREPROCESSORCONSUMER_H_
