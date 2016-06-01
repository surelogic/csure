#include "../ClangTidy.h"
#include "../ClangTidyModule.h"
#include "../ClangTidyModuleRegistry.h"
#include "../readability/BracesAroundStatementsCheck.h"
#include "../readability/FunctionSizeCheck.h"
#include "../readability/NamespaceCommentCheck.h"
#include "../readability/RedundantSmartptrGetCheck.h"
#include "EmptyDeclCheck.h"
#include "FixedCommentCheck.h"
#include "NullStmtCheck.h"
#include "UsingCheck.h"
#include "VoidReturnCheck.h"

using namespace clang::ast_matchers;

namespace clang {
namespace tidy {
namespace surelogic {

class SureLogicModule : public ClangTidyModule {
public:
  void addCheckFactories(ClangTidyCheckFactories &CheckFactories) override {
    CheckFactories.registerCheck<readability::EmptyDeclCheck>(
        "surelogic-readability-emptydecl");
    CheckFactories.registerCheck<readability::FixedCommentCheck>(
        "surelogic-readability-fixed");
    CheckFactories.registerCheck<readability::NullStmtCheck>(
        "surelogic-readability-nullstmt");
    CheckFactories.registerCheck<readability::UsingCheck>(
        "surelogic-readability-using");
    CheckFactories.registerCheck<readability::VoidReturnCheck>(
        "surelogic-readability-voidreturn");
  }
};

// Register the SureLogicTidyModule using this statically initialized variable.
static ClangTidyModuleRegistry::Add<SureLogicModule>
    X("surelogic-module", "Adds SureLogic analysis checks.");

} // namespace surelogic

// This anchor is used to force the linker to link in the generated object file
// and thus register the SureLogicModule.
volatile int SureLogicModuleAnchorSource = 0;

} // namespace tidy
} // namespace clang
