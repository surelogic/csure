#include <iostream>
#include <thread>

[[starts("nothing")]] void hello() {
  std::cout << "hello, world!" << std::endl;
}

class Point {
  const int x, y;

public:
  Point(int i, int j)
      : x(i),
        y(j){
            // Nothing else to do
        }

            [[starts("nothing")]] int getX() {
    return x;
  }

  [[starts("nothing")]] int getY() { return y; }

  [[starts("nothing")]] void unusual() {
    std::thread t{hello};
    t.join();
  }
};

[[starts("nothing")]] int main() {
  Point p(1, 2);
  p.unusual();
  return p.getX();
}
