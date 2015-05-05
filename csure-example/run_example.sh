#!/bin/bash
../../../../../llvm-360-build/bin/clang -Xclang -load \
      -Xclang ../../../../../llvm-360-build/lib/CSure.so \
      -Xclang -plugin -Xclang -example-plugin \
      -Wall -c test.c
