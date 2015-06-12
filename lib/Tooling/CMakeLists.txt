set(LLVM_LINK_COMPONENTS support)

file( GLOB_RECURSE headers
    ../../include/sl/Tooling/*.h
)

add_clang_library(csureTooling
  CSureASTConsumer.cc
  CSurePreprocessorConsumer.cc
  CSureToolAction.cc

  LINK_LIBS
  csureAnalysis
  clangAST
  clangBasic
  clangFrontend
  
  ADDITIONAL_HEADERS
  ${headers}
)
