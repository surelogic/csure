#!/bin/bash
# old setup
../../../../../llvm-360-build/bin/clang++ -std=c++11 -Xclang -load \
      -Xclang ../../../../../llvm-360-build/lib/CSure.so \
      -Xclang -plugin -Xclang -example-plugin \
      -Wall -c test.cc
