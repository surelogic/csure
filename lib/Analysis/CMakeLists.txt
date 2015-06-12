set(LLVM_LINK_COMPONENTS support)

file( GLOB_RECURSE headers
    ../../include/sl/Analysis/*.h
)

add_clang_library(csureAnalysis
  ExampleVisitor.cc

  LINK_LIBS
  clangAST
  clangBasic
  clangFrontend
  
  ADDITIONAL_HEADERS
  ${headers}
)
