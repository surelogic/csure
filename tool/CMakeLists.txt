set(LLVM_LINK_COMPONENTS support)
set(LLVM_USED_LIBS clangTooling clangBasic clangAST csureAnalysis)

add_clang_executable(csure-scan
  main.cpp
  )
target_link_libraries(csure-scan
  clangTooling
  clangBasic
  clangASTMatchers
  csureAnalysis
 )
