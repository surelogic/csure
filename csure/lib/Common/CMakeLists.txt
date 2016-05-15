set(LLVM_LINK_COMPONENTS support)

file( GLOB_RECURSE headers
    ../../include/sl/Common/*.h
)

add_clang_library(csureCommon
  SLUtil.cpp

  LINK_LIBS
  clangAST
  clangBasic
  clangFrontend
  
  ADDITIONAL_HEADERS
  ${headers}
)
