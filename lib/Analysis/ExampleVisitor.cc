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

#include "sl/Analysis/ExampleVisitor.h"

using namespace std;
using namespace clang;
using namespace llvm;

namespace sl {

	bool ExampleVisitor::isInLocalFile(SourceRange range) {
		FileID file = astContext->getSourceManager().getFileID(range.getBegin());
		const FileEntry* entry = astContext->getSourceManager().getFileEntryForID(file);
		if (entry == NULL) {
			/*
			errs() << "** Unable to get file entry for " <<
			range.getBegin().printToString(astContext->getSourceManager()) << "\n";
			*/
			return false;
		}
		const char* name = entry->getName();
		if (name == NULL) {
			return false;
		}
		return (name[0] != '/');
	}

	bool ExampleVisitor::VisitAttr(Attr *a) {
		if (isInLocalFile(a->getRange())) {
			errs() << "** Looking at attr: " << a->getSpelling() << "\n";
		}
		return true;
	}

	bool ExampleVisitor::VisitFunctionDecl(FunctionDecl *func) {
		string funcName = func->getNameInfo().getName().getAsString();
		SourceRange range = func->getSourceRange();

		if (isInLocalFile(range)) {
			/*
			errs() << "** " << funcName << ": "
			<< range.getBegin().printToString(rewriter.getSourceMgr())
			<< ", "
			<< range.getEnd().printToString(rewriter.getSourceMgr())
			<< "\n";

			rewriter.ReplaceText(func->getLocation(), funcName.length(), "add5");
			*/
			errs() << "** Looking at function def: " << funcName << "\n";

			for (auto a : func->attrs()) {
				errs() << "  ** Looking at attr on " << funcName << ": " << a->getSpelling() << "\n";
				switch (a->getKind()) {
				default:
					break;
				case attr::SureLogicStarts:
					SureLogicStartsAttr* sa = func->getAttr<SureLogicStartsAttr>();
					errs() << "  ** Got @Starts: " << sa->getValue() << "\n";
					break;
				}
			}
			//
		}
		return true;
	}

	bool ExampleVisitor::VisitCallExpr(CallExpr *c) {
		errs() << "** Looking at call:\n";
		c->dump(errs(), astContext->getSourceManager());
		errs() << "\n";

		errs() << "Callee decl:\n:";
		Decl* d = c->getCalleeDecl();
		d->dump(errs());
		errs() << "\n";

		for (auto a : d->attrs()) {
			errs() << "** Looking at attr on callee decl:" << a->getSpelling() << "\n";
		}
		return true;
	}

	bool ExampleVisitor::VisitStmt(Stmt *st) {
		/*
		if (ReturnStmt *ret = dyn_cast<ReturnStmt>(st)) {
		rewriter.ReplaceText(ret->getRetValue()->getLocStart(), 6, "val");
		errs() << "** Rewrote ReturnStmt\n";
		}
		if (CallExpr *call = dyn_cast<CallExpr>(st)) {
		rewriter.ReplaceText(call->getLocStart(), 7, "add5");
		errs() << "** Rewrote function call\n";
		}
		*/
		return true;
	}

	/*
		virtual bool VisitReturnStmt(ReturnStmt *ret) {
		rewriter.ReplaceText(ret->getRetValue()->getLocStart(), 6, "val");
		errs() << "** Rewrote ReturnStmt\n";
		return true;
		}

		virtual bool VisitCallExpr(CallExpr *call) {
		rewriter.ReplaceText(call->getLocStart(), 7, "add5");
		errs() << "** Rewrote function call\n";
		return true;
		}
		*/

	/*
	virtual bool VisitRecordDecl(RecordDecl *r) {
	errs() << "Found record: " << r->getDeclName().getAsString() << "\n";
	return true;
	}
	*/

	bool ExampleVisitor::VisitCXXRecordDecl(CXXRecordDecl *r) {
		if (isInLocalFile(r->getSourceRange())) {
			errs() << "Found CXX record: " << r->getDeclName().getAsString() << "\n";
		}
		return true;
	}

}
