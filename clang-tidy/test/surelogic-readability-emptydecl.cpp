// RUN: %check_clang_tidy %s surelogic-readability-emptydecl %t

void Method1() { } ; void Method2() { }
// CHECK-MESSAGES: :[[@LINE-1]]:20: warning: do not use empty declarations in a translation unit
// CHECK-FIXES: void Method1() { } void Method2() { }
