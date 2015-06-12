#include"sl/Tooling/CSurePreprocessorConsumer.h"

namespace sl {

	void CSurePreprocessorConsumer::InclusionDirective(clang::SourceLocation HashLoc,
		const clang::Token &IncludeTok,
		llvm::StringRef FileName,
		bool IsAngled,
		clang::CharSourceRange FilenameRange,
		const clang::FileEntry *File,
		llvm::StringRef SearchPath,
		llvm::StringRef RelativePath,
		const clang::Module *Imported) {
		llvm::errs() << " #include " << SearchPath << '/' << RelativePath << '\n';
	}

}
