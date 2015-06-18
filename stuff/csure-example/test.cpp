#include <vector>

class [[valueObject]] Point {
	const int x, y;

public:
	Point(int i, int j) : x(i), y(j) {
		// Nothing else to do
	}

	[[starts("nothing")]] int getX() {
		return x;
	}

	[[starts("nothing")]] int getY() {
		return y;
	}
};

class [[referenceObject]] UniqueID {

};

int main(void) {
  Point p(1, 2);
  return p.getX();
}

