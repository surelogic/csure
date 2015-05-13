#include <stdio.h>

//void do_math(int *x) __attribute__((clangShouldntKnowAboutThis)) {
[[starts("nothing")]] void do_math(int *x) /*__attribute__((deprecated))*/ {
  *x += 5;
}

int main(void) {
  int result = -1, val = 4;
  do_math(&val);
  return result;
}

class [[valueObject]] Foo {

};
