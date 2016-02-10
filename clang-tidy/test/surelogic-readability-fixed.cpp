// RUN: %check_clang_tidy %s surelogic-readability-fixed %t

void f(); // FIXED by me!
// CHECK-MESSAGES:[[@LINE-1]]:11: warning: missing username/bug in FIXED [surelogic-readability-fixed]
// CHECK-FIXES: void f(); // FIXED(unknown): by me!

void awesome_f2(); // FIXED(joeprogrammer) FIXED(465)
