set(LLVM_LINK_COMPONENTS support)

file( GLOB_RECURSE headers
    ../../include/sl/Tooling/*.h
)

file( GLOB_RECURSE sources
    *.cc
)

add_clang_library(csureTooling
  ${sources}
#  CSureASTConsumer.cc
#  CSurePreprocessorConsumer.cc
#  CSureToolAction.cc
  
  LINK_LIBS
  csureAnalysis
  clangAST
  clangBasic
  clangFrontend
  
  ADDITIONAL_HEADERS
  ${headers}
)