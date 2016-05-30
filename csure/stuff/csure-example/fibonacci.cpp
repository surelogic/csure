namespace math {

[[starts("nothing")]] int fib(int x) {
  if (x == 1) {
    return 1;
  } else {
    return fib(x - 1) + fib(x - 2);
  }
}

[[starts("nothing")]] int main() {
  int result = fib(5);
  return 0;
}

} // namespace math
