set(LLVM_LINK_COMPONENTS support)

file( GLOB_RECURSE headers
    ../../include/sl/Sea/*.h
)

add_clang_library(csureSea
  AnalysisResultDrop.cpp
  Drop.cpp
  PromiseDrop.cpp
  ProofDrop.cpp
  ResultDrop.cpp
  ResultFolderDrop.cpp
  Sea.cpp

  LINK_LIBS
  csureCommon
  
  ADDITIONAL_HEADERS
  ${headers}
)
