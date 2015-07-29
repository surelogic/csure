set(LLVM_LINK_COMPONENTS support)

file( GLOB_RECURSE headers
    ../../include/sl/Tooling/*.h
)

file( GLOB_RECURSE sources
    *.cc
)

add_clang_library(csureTooling
  ${sources}
  
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
