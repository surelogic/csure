set(LLVM_LINK_COMPONENTS support)

add_clang_library(csure
  CSure.cpp

  LINK_LIBS
  clangBasic
  clangLex
  )
