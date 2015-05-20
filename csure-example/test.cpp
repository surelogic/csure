#include <stdio.h>

//void do_math(int *x) __attribute__((clangShouldntKnowAboutThis)) {
[[starts("nothing")]] void do_math(int *x) /*__attribute__((deprecated))*/ {
  *x += 5;
}

void do_math(int* y);

class [[valueObject]] [[referenceObject]] Foo {
public:
	[[starts("nothing")]] int bar(int i) {
		return 0;
	}
};

int main(void) {
  int result = -1, val = 4;
  do_math(&val);

  Foo f;
  f.bar(0);
  return result;
}

