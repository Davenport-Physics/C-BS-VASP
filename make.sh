#!/bin/bash

cd src
gcc -Wall -O3 -o CBSVASP *.c
cd ..
mv src/CBSVASP CBSVASP
