#include <thread>

// This should generate a warning about the promise
[[starts("hello")]] void hello() { std::thread t{hello}; }
