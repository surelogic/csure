set(LLVM_LINK_COMPONENTS support)

file( GLOB_RECURSE headers
    ../../include/sl/Tooling/*.h
)

add_clang_library(csureTooling
  CSureASTConsumer.cpp
  CSurePreprocessorConsumer.cpp
  CSureToolAction.cpp
  
  LINK_LIBS
  csureAnalysis
  csureCommon
  csureSea
  clangAST
  clangBasic
  clangFrontend
  
  ADDITIONAL_HEADERS
  ${headers}
)
