#include "sl/Tooling/CSureToolAction.h"

std::unique_ptr<clang::ASTConsumer> CSureToolAction::CreateASTConsumer(clang::CompilerInstance &CI, llvm::StringRef file) {
	//TODO CSurePreprocessorInfo* preprocessor_consumer = new CSurePreprocessorInfo();

	CI.getPreprocessor().addPPCallbacks(std::unique_ptr<clang::PPCallbacks>(preprocessor_consumer));
	return std::unique_ptr<clang::ASTConsumer>(new ExampleASTConsumer(&CI));
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
