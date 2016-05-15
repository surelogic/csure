set(LLVM_LINK_COMPONENTS support)

add_clang_library(clangTidySureLogicModule
  SureLogicTidyModule.cpp
  FixedCommentCheck.cpp
  UsingCheck.cpp
  NullStmtCheck.cpp
  EmptyDeclCheck.cpp
  HeaderFileExtensionsUtils.cpp

  LINK_LIBS
  clangAST
  clangASTMatchers
  clangBasic
  clangLex
  clangTidy
  clangTidyReadabilityModule
  )
