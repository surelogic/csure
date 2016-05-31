#!/bin/bash
echo "----- Restore edns.c from ./original/edns.c"
cp original/edns.c .
echo "----- Use clang-tidy surelogic-readability-emptydecl,surelogic-readability-nullstmt with -fix-errors flag"
clang-tidy -fix-errors -checks=-*,surelogic-readability-emptydecl,surelogic-readability-nullstmt edns.c
echo "----- Use clang-format to reformat the output"
clang-format -i -style=file edns.c
echo "----- Compile the output"
clang edns.c -o edns
echo "----- Run the new program"
./edns
#!/bin/bash
