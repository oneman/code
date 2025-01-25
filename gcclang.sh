#!/bin/sh

git log | grep "rawdod" | wc -l > VERSION
gcc -Werror clock.c
./a.out > clog1
mv a.out bin/gcc.out
clang -Wall clock.c
./a.out > clog2
mv a.out bin/clang.out
gcc --version
cat clog1
clang --version
cat clog2
diff clog1 clog2
