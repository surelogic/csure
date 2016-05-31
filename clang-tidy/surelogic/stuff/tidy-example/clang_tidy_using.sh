#!/bin/bash
echo "----- Restore using.cpp from ./original/using.cpp"
cp original/using.cpp .
echo "----- Use clang-tidy surelogic-readability-using with -fix-errors flag"
clang-tidy -fix-errors -checks=-*,surelogic-readability-using using.cpp -- -std=c++11
echo "----- Use clang-format to reformat the output"
clang-format -i -style=file using.cpp
echo "----- Compile the output"
clang++ -std=c++11 using.cpp -o using
echo "----- Run the new program"
./using
