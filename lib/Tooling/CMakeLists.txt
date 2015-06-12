set(LLVM_LINK_COMPONENTS support)

file( GLOB_RECURSE headers
    ../../include/csure/Tooling/*.h
)

add_clang_library(csureTooling
  CSureToolAction.cc

  LINK_LIBS
  clangAST
  clangBasic
  clangFrontend
  csureAnalysis
  
  ADDITIONAL_HEADERS
  ${headers}
)
