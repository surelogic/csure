#!/bin/bash
clang -Xclang -load \
      -Xclang C++11/libCSure.so \
      -Xclang -plugin -Xclang -example-plugin \
      -Wall -c test.c
