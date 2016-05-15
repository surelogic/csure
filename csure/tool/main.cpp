#include "clang/Driver/Options.h"
#include "clang/AST/AST.h"
#include "clang/AST/ASTContext.h"
#include "clang/AST/ASTConsumer.h"
#include "clang/AST/RecursiveASTVisitor.h"
#include "clang/Frontend/ASTConsumers.h"
#include "clang/Frontend/FrontendActions.h"
#include "clang/Frontend/CompilerInstance.h"
#include "clang/Tooling/CommonOptionsParser.h"
#include "llvm/Support/CommandLine.h"
#include "clang/Tooling/Tooling.h"
#include "clang/Rewrite/Core/Rewriter.h"

#include "sl/Tooling/CSureToolAction.h"

static llvm::cl::OptionCategory csure_option_category{"CSure option category"};

int main(int argc, const char **argv) {
  // parse the command-line args
  clang::tooling::CommonOptionsParser op{argc, argv, csure_option_category};

  clang::tooling::ClangTool tool{op.getCompilations(), op.getSourcePathList()};

  const int result{tool.run(clang::tooling::newFrontendActionFactory<sl::CSureToolAction>().get())};
  return result;
}
