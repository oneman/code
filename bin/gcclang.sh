#!/bin/sh

git log | grep "rawdod" | wc -l > VERSION

gcc --version | head -n 1
gcc -Werror clock.c
./a.out > clog1
mv a.out bin/gcc.out
cat clog1

clang --version | head -n 1
clang -Wall clock.c
./a.out > clog2
mv a.out bin/clang.out
cat clog2

diff clog1 clog2
