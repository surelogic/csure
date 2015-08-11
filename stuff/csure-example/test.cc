//#include <vector>
#include <iostream>
#include <thread>

[[starts("nothing")]] 
void hello() {
  std::cout << "hello, world!" << std::endl;
}

class[[valueObject]] Point {
  const int x, y;

 public:
  Point(int i, int j) : x(i), y(j) {
    // Nothing else to do
  }

  [[starts("nothing")]] int getX() { return x; }

  [[starts("nothing")]] int getY() { return y; }

  [[starts("nothing")]] // illegal annotation
  void unusual() {
    std::thread t {hello};
    t.join();
  }

};

class[[referenceObject]] UniqueID{

};

[[starts("nothing")]] // unsafe because constructor of Point is not annotated
int main() {
  Point p(1, 2); 
  return p.getX();
}
