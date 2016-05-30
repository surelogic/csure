#include <iostream>
#include <thread>

[[starts("nothing")]] void hello() {
  std::cout << "hello, world!" << std::endl;
}

class Point {
  int x, y;

public:
  Point(int i, int j)
      : x(i), y(j){}

  [[starts("nothing")]] int getX() {
    return x;
  }

  [[starts("nothing")]] int setX(const int value) {
    const int old = x;
    x = value;
    return old;
  }

  [[starts("nothing")]] int getY() { return y; }

  [[starts("nothing")]] int setY(const int value) {
    const int old = y;
    y = value;
    return old;
  }

  [[starts("nothing")]] void unusual() {
    setX(2);
    setY(3);
    std::thread t{hello};
    t.join();
  }
};

[[starts("nothing")]] int main() {
  Point p(1, 2);
  p.unusual();
  return p.getX();
}
