#pragma once

#include <memory>
#include <vector>
#include <string>

#include "clang/AST/ASTConsumer.h"
#include "clang/Frontend/FrontendActions.h"

class CSureToolAction : public clang::PluginASTAction {
protected:
	// this gets called by Clang when it invokes CSure
	std::unique_ptr<clang::ASTConsumer> CreateASTConsumer(clang::CompilerInstance &CI, llvm::StringRef file);

	// implement this function if you want to parse custom cmd-line args
	bool ParseArgs(const clang::CompilerInstance &CI, const std::vector<std::string> &args);

	virtual  bool BeginSourceFileAction(clang::CompilerInstance &CI, llvm::StringRef Filename) override;
};

