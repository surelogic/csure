#!/bin/bash
# This will only work if the tool is installed an on your path.
# If this is not the case set an absolute or relative path on
# the surelogic-csure command, such as:
#  <llvm root>/build/bin/surelogic-csure
echo "-----------------------------------------------------"
surelogic-csure point.cpp -- -std=c++11 -c
echo "-----------------------------------------------------"
surelogic-csure fibonacci.cpp -- -std=c++11 -c
echo "-----------------------------------------------------"
echo "(Note: the next test should generate a warning.)"
surelogic-csure warning.cpp -- -std=c++11 -c
echo "-----------------------------------------------------"
