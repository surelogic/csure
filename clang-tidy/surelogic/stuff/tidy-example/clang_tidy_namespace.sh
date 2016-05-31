#!/bin/bash
echo "----- Restore namespace.cpp from ./original/namespace.cpp"
cp original/namespace.* .
echo "----- Use clang-tidy surelogic-readability-using with -fix-errors flag"
clang-tidy -fix-errors -checks=-*,surelogic-readability-using namespace.cpp -- -std=c++11
echo "----- Use clang-format to reformat the output"
clang-format -i -style=file namespace.cpp
echo "----- Compile the output"
clang++ -std=c++11 namespace.cpp -o namespace
echo "----- Run the new program"
./namespace
