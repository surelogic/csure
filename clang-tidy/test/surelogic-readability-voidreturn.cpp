// RUN: %check_clang_tidy %s surelogic-readability-voidreturn %t

void Method1() { } void Method2() {return Method1();}
// CHECK-MESSAGES: :[[@LINE-1]]:43: warning: do not return a void expression from a function
// CHECK-FIXES: void Method1() { } void Method2() {Method1(); return;}
