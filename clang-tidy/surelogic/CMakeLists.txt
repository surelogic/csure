set(LLVM_LINK_COMPONENTS support)

add_clang_library(clangTidySureLogicModule
  SureLogicTidyModule.cpp
  HeaderFileExtensionsUtils.cpp
  EmptyDeclCheck.cpp
  FixedCommentCheck.cpp
  NullStmtCheck.cpp
  UsingCheck.cpp
  VoidReturnCheck.cpp

  LINK_LIBS
  clangAST
  clangASTMatchers
  clangBasic
  clangLex
  clangTidy
  clangTidyReadabilityModule
  )
