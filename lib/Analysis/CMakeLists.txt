set(LLVM_LINK_COMPONENTS support)

file( GLOB_RECURSE headers
    ../../include/csure/Analysis/*.h
)

add_clang_library(csureAnalysis
  CSure.cpp

  LINK_LIBS
  clangAST
  clangBasic
  clangFrontend
  
  ADDITIONAL_HEADERS
  ${headers}
  )
