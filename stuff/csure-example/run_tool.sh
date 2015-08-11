#!/bin/bash
${LLVMBUILD}/bin/surelogic-csure test.cc -- ${LLVMBUILD}/bin/clang++ \
 -I /opt/local/include/gcc49/c++ \
 -I /opt/local/include/gcc49/c++/x86_64-apple-darwin12 \
 -I /opt/local/include/gcc49/c++//backward \
 -std=c++14 -c
