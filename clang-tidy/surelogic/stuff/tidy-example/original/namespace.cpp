#include <iostream>
#include <string>

#include "namespace.h"

using namespace surelogic;
using namespace std;

int main() {
  cout << "default x=" << GetDefault().x << "\n";
  cout << "default x=" << DEF_X << "\n";
  cout << "default y=" << GetDefault().y << "\n";
  cout << "default x=" << DEF_Y << "\n";
  Point p;
  p.x = 5;
  cout << "p.x=" << p.x << "\n";
  string end{"finished"};
  cout << end << "\n";
}
