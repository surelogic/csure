#include <iostream>
#include <thread>

[[starts("nothing")]] void hello() {
  std::cout << "hello, world!" << std::endl;
}

class Point {
public:
  Point(int i, int j)
      : x(i), y(j){}

                  [[starts("nothing")]] int getX() {
    return x;
  }

  [[starts("nothing")]] void setX(const int value) { x = value; }

  [[starts("nothing")]] int getY() { return y; }

  [[starts("nothing")]] void setY(const int value) { y = value; }

  [[starts("nothing")]] void unusual() {
    setX(2);
    setY(3);
    // BAD - starts thread!
    std::thread t{hello};
    t.join();
  }

private:
  int x, y;
};

[[starts("nothing")]] int main() {
  Point p(1, 2);
  p.unusual();
  return p.getX();
}
