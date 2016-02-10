set(LLVM_LINK_COMPONENTS support)

file( GLOB_RECURSE headers
    ../../include/sl/Common/*.h
)

file( GLOB_RECURSE sources
    *.cc
)

add_clang_library(csureCommon
  ${sources}

  LINK_LIBS
  clangAST
  clangBasic
  clangFrontend
  
  ADDITIONAL_HEADERS
  ${headers}
)
