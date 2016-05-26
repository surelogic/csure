set(LLVM_LINK_COMPONENTS support)

file( GLOB_RECURSE headers
    ../../include/sl/Sea/*.h
)

add_clang_library(csureSea
  Sea.cpp
  Drop.cpp
  ProofDrop.cpp
  PromiseDrop.cpp

  LINK_LIBS
  csureCommon
  
  ADDITIONAL_HEADERS
  ${headers}
)
