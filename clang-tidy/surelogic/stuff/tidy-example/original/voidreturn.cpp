#include <iostream>

void f2() {
  std::cout << "void return\n";
}
void f1() {
  return f2();
}

int main() {
  f1();
  return 0;
}
