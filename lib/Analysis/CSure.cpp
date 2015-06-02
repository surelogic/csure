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

#include "csure/Analysis/CSure.h"

using namespace std;
using namespace clang;
using namespace llvm;

//Rewriter rewriter;
int numFunctions = 0;


class ExampleVisitor : public RecursiveASTVisitor<ExampleVisitor> {
private:
    ASTContext *astContext; // used for getting additional AST info

    ~ExampleVisitor() {
    	// What is there to do here?
    }

public:
    explicit ExampleVisitor(CompilerInstance *CI) 
      : astContext(&(CI->getASTContext())) // initialize private members
    {
        //rewriter.setSourceMgr(astContext->getSourceManager(), astContext->getLangOpts());
    }

    bool isInLocalFile(SourceRange range) {
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

    virtual bool VisitAttr(Attr *a) /*override*/ {
    	if (isInLocalFile(a->getRange())) {
    		errs() << "** Looking at attr: " << a->getSpelling() << "\n";
    	}
    	return true;
    }

    virtual bool VisitFunctionDecl(FunctionDecl *func) {
        numFunctions++;
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

            for(auto a : func->attrs()) {
            	errs() << "  ** Looking at attr on " << funcName << ": " << a->getSpelling() << "\n";
            	switch (a->getKind()) {
            	default:
            		break;
            	case attr::SureLogicStarts:
            		SureLogicStartsAttr* sa =  func->getAttr<SureLogicStartsAttr>();
            		errs() << "  ** Got @Starts: " << sa->getValue() << "\n";
            		break;
            	}
            }
            //
        }    
        return true;
    }

    virtual bool VisitCallExpr(CallExpr *c) {
    	errs() << "** Looking at call:\n";
    	c->dump(errs(), astContext->getSourceManager());
    	errs() << "\n";

    	errs() << "Callee decl:\n:";
    	Decl* d = c->getCalleeDecl();
    	d->dump(errs());
    	errs() << "\n";

    	for(auto a : d->attrs()) {
    	 	errs() << "** Looking at attr on callee decl:" << a->getSpelling() << "\n";
    	}
    	return true;
    }

    virtual bool VisitStmt(Stmt *st) {
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

    virtual bool VisitCXXRecordDecl(CXXRecordDecl *r) {
    	if (isInLocalFile(r->getSourceRange())) {
    		errs() << "Found CXX record: " << r->getDeclName().getAsString() << "\n";
    	}
    	return true;
    }
};



class ExampleASTConsumer : public ASTConsumer {
private:
    ExampleVisitor *visitor; // doesn't have to be private

public:
    // override the constructor in order to pass CI
    explicit ExampleASTConsumer(CompilerInstance *CI)
        : visitor(new ExampleVisitor(CI)) // initialize the visitor
    { }

    // override this to call our ExampleVisitor on the entire source file
    virtual void HandleTranslationUnit(ASTContext &Context) {
        /* we can use ASTContext to get the TranslationUnitDecl, which is
             a single Decl that collectively represents the entire source file */
        visitor->TraverseDecl(Context.getTranslationUnitDecl());
    }

/*
    // override this to call our ExampleVisitor on each top-level Decl
    virtual bool HandleTopLevelDecl(DeclGroupRef DG) {
        // a DeclGroupRef may have multiple Decls, so we iterate through each one
        for (DeclGroupRef::iterator i = DG.begin(), e = DG.end(); i != e; i++) {
            Decl *D = *i;    
            visitor->TraverseDecl(D); // recursively visit each AST node in Decl "D"
        }
        return true;
    }
*/
};

class CSurePreprocessorInfo : public clang::PPCallbacks {
	~CSurePreprocessorInfo() {
		// What to do here?
	}

	virtual void InclusionDirective(SourceLocation HashLoc,
	                                  const Token &IncludeTok,
	                                  StringRef FileName,
	                                  bool IsAngled,
	                                  CharSourceRange FilenameRange,
	                                  const FileEntry *File,
	                                  StringRef SearchPath,
	                                  StringRef RelativePath,
	                                  const Module *Imported) override {
		errs() << "** Including " << SearchPath << '/' << RelativePath << "\n";
	}
};

/*
class PluginExampleAction : public PluginASTAction {
protected:
    // this gets called by Clang when it invokes our Plugin
	std::unique_ptr<ASTConsumer> CreateASTConsumer(CompilerInstance &CI, StringRef file) {
		CSurePreprocessorInfo* preprocessor_consumer = new CSurePreprocessorInfo();

	    CI.getPreprocessor().addPPCallbacks(std::unique_ptr<PPCallbacks>(preprocessor_consumer));
        return std::unique_ptr<ASTConsumer>(new ExampleASTConsumer(&CI));
    }

    // implement this function if you want to parse custom cmd-line args
    bool ParseArgs(const CompilerInstance &CI, const vector<string> &args) {
    	errs() << "** Working dir? " << CI.getFileSystemOpts().WorkingDir << "\n";
    	errs() << "** Output going to " << CI.getFrontendOpts().OutputFile << "\n";
        return true;
    }

    virtual  bool BeginSourceFileAction(CompilerInstance &CI,
                                        StringRef Filename) override {
       errs() << "** Looking at " << Filename << "\n";
       return PluginASTAction::BeginSourceFileAction(CI, Filename);
    }
};
*/

// this gets called by Clang when it invokes our Plugin
std::unique_ptr<ASTConsumer> PluginExampleAction::CreateASTConsumer(CompilerInstance &CI, StringRef file) {
	CSurePreprocessorInfo* preprocessor_consumer = new CSurePreprocessorInfo();

    CI.getPreprocessor().addPPCallbacks(std::unique_ptr<PPCallbacks>(preprocessor_consumer));
    return std::unique_ptr<ASTConsumer>(new ExampleASTConsumer(&CI));
}

// implement this function if you want to parse custom cmd-line args
bool PluginExampleAction::ParseArgs(const CompilerInstance &CI, const vector<string> &args) {
	errs() << "** Working dir? " << CI.getFileSystemOpts().WorkingDir << "\n";
	errs() << "** Output going to " << CI.getFrontendOpts().OutputFile << "\n";
    return true;
}

bool PluginExampleAction::BeginSourceFileAction(CompilerInstance &CI,
                                    StringRef Filename) {
   errs() << "** Looking at " << Filename << "\n";
   return PluginASTAction::BeginSourceFileAction(CI, Filename);
}

//static FrontendPluginRegistry::Add<PluginExampleAction> X("-example-plugin", "simple Plugin example");
