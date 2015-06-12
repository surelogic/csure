#include "clang/lex/Preprocessor.h"

#include "sl/Tooling/CSureToolAction.h"
#include "sl/Tooling/CSurePreprocessorConsumer.h"

namespace sl {

	std::unique_ptr<clang::ASTConsumer>  CSureToolAction::CreateASTConsumer(clang::CompilerInstance &CI, llvm::StringRef file) {
		CI.getPreprocessor().addPPCallbacks(std::unique_ptr < clang::PPCallbacks > {new CSurePreprocessorConsumer()});
		return nullptr;//std::unique_ptr<clang::ASTConsumer>(new ExampleASTConsumer(&CI));
	}

	bool CSureToolAction::ParseArgs(const clang::CompilerInstance &CI, const std::vector<std::string> &args) {
		llvm::errs() << "** Working dir? " << CI.getFileSystemOpts().WorkingDir << "\n";
		llvm::errs() << "** Output going to " << CI.getFrontendOpts().OutputFile << "\n";
		return true;
	}

	bool CSureToolAction::BeginSourceFileAction(clang::CompilerInstance &CI,
		llvm::StringRef Filename) {
		llvm::errs() << "** Looking at " << Filename << "\n";
		return PluginASTAction::BeginSourceFileAction(CI, Filename);
	}

}
