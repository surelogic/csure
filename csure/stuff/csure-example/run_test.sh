#!/bin/bash
# Make sure build/bin is in your path -- see Wiki for 'use-my-clang.sh' script
surelogic-csure test.cpp -- clang++ -std=c++14 -c
