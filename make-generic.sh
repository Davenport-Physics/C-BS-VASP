#!/bin/bash

#Generates generic 64 bit version of CBSVASP
cd src
gcc -mtune=generic -m64 -o CBSVASP-Generic *.c
cd ..
cp src/CBSVASP-Generic CBSVASP-Generic
