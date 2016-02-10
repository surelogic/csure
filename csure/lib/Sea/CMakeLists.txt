set(LLVM_LINK_COMPONENTS support)

file( GLOB_RECURSE headers
    ../../include/sl/Sea/*.h
)

file( GLOB_RECURSE sources
    *.cc
)

add_clang_library(csureSea
  ${sources}

  LINK_LIBS
  csureCommon
  
  ADDITIONAL_HEADERS
  ${headers}
)
