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

/*!
 * @file
 *
 * \mainpage notitle
 *
 * The CSure tool is a prototype analysis-based verification system. It is focused
 * on proving mechanical properties of C and C++ programs given specification
 * fragments in the form of C++11 annotations.
 *
 * The current prototype includes a C++ implementation of the drop-sea truth
 * maintenance system, an automated theorem prover, and an analysis to verify the
 * [[starts("nothing")]] annotation. The latter states that the code may not
 * start a thread.
 */

static llvm::cl::OptionCategory csure_option_category{"CSure option category"};

/// \brief The main program for the CSure tool.
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
