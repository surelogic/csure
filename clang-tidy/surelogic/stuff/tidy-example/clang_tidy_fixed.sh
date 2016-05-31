#!/bin/bash
echo "----- Restore using.cpp from ./original/fixed.cpp"
cp original/fixed.cpp .
echo "----- Use clang-tidy surelogic-readability-fixed with -fix-errors flag"
clang-tidy -fix-errors -checks=-*,surelogic-readability-fixed fixed.cpp -- -std=c++11
echo "----- Use clang-format to reformat the output"
clang-format -i -style=file fixed.cpp
echo "----- Compile the output"
clang++ -std=c++11 fixed.cpp -o fixed
echo "----- Run the new program"
./fixed
