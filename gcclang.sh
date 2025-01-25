#!/bin/sh

gcc -Werror clock.c
./a.out > clog1
rm a.out
clang -Wall clock.c
./a.out > clog2
rm a.out
diff clog1 clog2
cat clog1
