#include <iostream>
#include <string>

#include "namespace.h"

int main() {
  std::cout << "default x=" << surelogic::GetDefault().x << "\n";
  std::cout << "default x=" << surelogic::DEF_X << "\n";
  std::cout << "default y=" << surelogic::GetDefault().y << "\n";
  std::cout << "default x=" << surelogic::DEF_Y << "\n";
  surelogic::Point p;
  p.x = 5;
  std::cout << "p.x=" << p.x << "\n";
  std::string end{"finished"};
  std::cout << end << "\n";
}
