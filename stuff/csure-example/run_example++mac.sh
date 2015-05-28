#!/bin/bash
#      -I/Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX10.10.sdk/System/Library/Frameworks/Kernel.framework/Versions/A/Headers/ \
../../../../../build/Debug/bin/clang++ -std=c++11 -Xclang -load \
      -I/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/include/c++/v1 \
      -Xclang ../../../../../build/Debug/lib/CSure.dylib \
      -Xclang -plugin -Xclang -example-plugin \
      -Wall -c test.cpp
