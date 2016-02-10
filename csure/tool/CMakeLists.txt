set(LLVM_LINK_COMPONENTS support)
set(LLVM_USED_LIBS csureTooling csureAnalysis clangTooling clangBasic clangAST)

add_clang_executable(surelogic-csure
  main.cc
  )
target_link_libraries(surelogic-csure
  csureTooling
  csureAnalysis
  clangTooling
  clangBasic
  clangASTMatchers
 )
