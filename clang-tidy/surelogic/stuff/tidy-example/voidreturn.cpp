#include <iostream>

void f2() { std::cout << "void return\n"; }
void f1() {
  f2();
  return;
}

int main() {
  f1();
  return 0;
}
