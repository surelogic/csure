#ifndef LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_SURELOGIC_SLDEBUG_H_
#define LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_SURELOGIC_SLDEBUG_H_

// Add "#define SL_DEBUG" to your program to get debug output before you
// include this file.
//
// For example:
//
// #define SL_DEBUG
// #include "sl/Common/SLDebug.h"
//
// void f() {
//   D(l() << "my debug output\n";)
// }
#ifdef SL_DEBUG
#define D(x) x
#else
#define D(x)
#endif

#endif //  LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_SURELOGIC_SLDEBUG_H_
