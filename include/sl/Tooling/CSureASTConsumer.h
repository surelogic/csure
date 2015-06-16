#pragma once

#include "clang/AST/ASTConsumer.h"
#include "clang/AST/ASTContext.h"

namespace sl {

	class CSureASTConsumer : public clang::ASTConsumer {

		virtual void HandleTranslationUnit(clang::ASTContext &Ctx) override;

	};

}
