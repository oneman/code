#!/bin/sh
echo "Running Super Set Up ..."
date +%s
echo "Running gcc ..."
gcc clock.c
echo "Done with gcc"
sleep 0.1
echo "Just a second ..."
sleep 1
echo "OK"
sleep 0.1
./a.out
echo "Clock ran"
date +%s
echo "Script Complete."
