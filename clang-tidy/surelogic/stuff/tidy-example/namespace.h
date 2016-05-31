namespace surelogic {

class Point {
public:
  int x;
  int y;
  void Set(const int x, const int y) {
    this->x = x;
    this->y = y;
  }
};

static int DEF_X = 1;
static int DEF_Y = 1;

static Point GetDefault() {
  Point p;
  p.Set(DEF_X, DEF_Y);
  return p;
}

} // namespace surelogic
