set(LLVM_LINK_COMPONENTS support)

file( GLOB_RECURSE headers
    ../../include/sl/Analysis/*.h
)

add_clang_library(csureAnalysis
  ExampleVisitor.cpp
  StartThreadAnalysis.cpp

  LINK_LIBS
  csureCommon
  csureSea
  clangAST
  clangBasic
  clangFrontend
  
  ADDITIONAL_HEADERS
  ${headers}
)
