// RUN: %check_clang_tidy %s surelogic-readability-nullstmt %t

void Method1() { ; }
// CHECK-MESSAGES: :[[@LINE-1]]:18: warning: do not use null or empty statments in functions
// CHECK-FIXES: void Method1() { }
