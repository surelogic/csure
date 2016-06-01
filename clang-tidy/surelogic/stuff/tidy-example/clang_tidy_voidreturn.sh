#!/bin/bash
echo "----- Restore voidreturn.cpp from ./original/voidreturn.cpp"
cp original/voidreturn.cpp .
echo "----- Use clang-tidy surelogic-readability-using with -fix-errors flag"
clang-tidy -fix-errors -checks=-*,surelogic-readability-voidreturn voidreturn.cpp -- -std=c++11
echo "----- Use clang-format to reformat the output"
clang-format -i -style=file voidreturn.cpp
echo "----- Compile the output"
clang++ -std=c++11 voidreturn.cpp -o voidreturn
echo "----- Run the new program"
./voidreturn
