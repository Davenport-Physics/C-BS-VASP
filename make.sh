#!/bin/bash

cd src
gcc -Wall -O3 -o eigenval *.c
cd ..
mv src/eigenval eigenval
