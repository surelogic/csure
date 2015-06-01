set(LLVM_LINK_COMPONENTS support)

add_clang_library(csureAnalysis
  CSure.cpp

  LINK_LIBS
  clangAST
  clangBasic
  clangFrontend
  )
