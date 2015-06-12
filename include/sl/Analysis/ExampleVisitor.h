#include <memory>
#include <utility>
#include "clang/Driver/Options.h"
#include "clang/AST/AST.h"
#include "clang/AST/ASTContext.h"
#include "clang/AST/ASTConsumer.h"
#include "clang/AST/Attr.h"
#include "clang/AST/RecursiveASTVisitor.h"
#include "clang/Basic/AttrKinds.h"
#include "clang/Frontend/ASTConsumers.h"
#include "clang/Frontend/FrontendActions.h"
#include "clang/Frontend/CompilerInstance.h"
#include "clang/Frontend/FrontendPluginRegistry.h"
#include "clang/Rewrite/Core/Rewriter.h"
#include "clang/Lex/Preprocessor.h"

using namespace std;
using namespace clang;
using namespace llvm;

namespace sl {

	class ExampleVisitor : public clang::RecursiveASTVisitor < ExampleVisitor > {
	private:
		ASTContext *astContext; // used for getting additional AST info

		~ExampleVisitor() {
			// What is there to do here?
		}

	public:
		explicit ExampleVisitor(CompilerInstance *CI) : astContext{ &(CI->getASTContext()) } {}

		bool isInLocalFile(SourceRange range);

		virtual bool VisitAttr(Attr *a);

		virtual bool VisitFunctionDecl(FunctionDecl *func);

		virtual bool VisitCallExpr(CallExpr *c) override;

		virtual bool VisitStmt(Stmt *st);

		virtual bool VisitCXXRecordDecl(CXXRecordDecl *r);
	};
}