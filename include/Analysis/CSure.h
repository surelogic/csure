/*
 * CSure.h
 *
 *  Created on: May 29, 2015
 *      Author: edwin
 */

#ifndef TOOLS_CLANG_CSURE_INCLUDE_ANALYSIS_CSURE_H_
#define TOOLS_CLANG_CSURE_INCLUDE_ANALYSIS_CSURE_H_

#include "clang/AST/ASTConsumer.h"
#include "clang/Frontend/FrontendActions.h"

using namespace std;
using namespace clang;
using namespace llvm;

//class PluginExampleAction;
class PluginExampleAction : public PluginASTAction {
protected:
    // this gets called by Clang when it invokes our Plugin
	std::unique_ptr<ASTConsumer> CreateASTConsumer(CompilerInstance &CI, StringRef file);

    // implement this function if you want to parse custom cmd-line args
    bool ParseArgs(const CompilerInstance &CI, const vector<string> &args);

    virtual  bool BeginSourceFileAction(CompilerInstance &CI,
                                        StringRef Filename) override ;
};

#endif /* TOOLS_CLANG_CSURE_INCLUDE_ANALYSIS_CSURE_H_ */
