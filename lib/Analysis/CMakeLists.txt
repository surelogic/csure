set(LLVM_LINK_COMPONENTS support)

file( GLOB_RECURSE headers
    ../../include/sl/Analysis/*.h
)

file( GLOB_RECURSE sources
    *.cc
)

add_clang_library(csureAnalysis
  ${sources}

  LINK_LIBS
  csureCommon
  clangAST
  clangBasic
  clangFrontend
  
  ADDITIONAL_HEADERS
  ${headers}
)
