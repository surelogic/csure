#include "clang/AST/AST.h"
#include "clang/AST/ASTConsumer.h"
#include "clang/AST/ASTContext.h"
#include "clang/AST/RecursiveASTVisitor.h"
#include "clang/Driver/Options.h"
#include "clang/Frontend/ASTConsumers.h"
#include "clang/Frontend/CompilerInstance.h"
#include "clang/Frontend/FrontendActions.h"
#include "clang/Rewrite/Core/Rewriter.h"
#include "clang/Tooling/CommonOptionsParser.h"
#include "clang/Tooling/Tooling.h"
#include "llvm/Support/CommandLine.h"

#include "sl/Common/SLUtil.h"
#include "sl/Sea/Sea.h"
#include "sl/Tooling/CSureToolAction.h"

static llvm::cl::OptionCategory csure_option_category{"CSure option category"};

// The main program for the CSure tool.
int main(int argc, const char **argv) {
  // parse the command-line args
  clang::tooling::CommonOptionsParser op{argc, argv, csure_option_category};

  clang::tooling::ClangTool tool{op.getCompilations(), op.getSourcePathList()};

  const int result{tool.run(
      clang::tooling::newFrontendActionFactory<sl::CSureToolAction>().get())};
  sl::Sea::Default()->UpdateConsistencyProof();
  sl::l() << sl::Sea::Default()->ToString();
  return result;
}
