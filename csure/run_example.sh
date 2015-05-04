#!/bin/bash
clang -Xclang -load \
      -Xclang C++11/libPluginExample.so \
      -Xclang -plugin -Xclang -example-plugin \
      -Wall -c test.c
