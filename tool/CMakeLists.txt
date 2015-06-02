set(LLVM_LINK_COMPONENTS support)
set(LLVM_USED_LIBS clangTooling clangBasic clangAST csureAnalysis)

add_clang_executable(surelogic-csure
  main.cpp
  )
target_link_libraries(surelogic-csure
  clangTooling
  clangBasic
  clangASTMatchers
  csureAnalysis
 )
