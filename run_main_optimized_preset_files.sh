#!/bin/sh
make clean
clear
make main
./main O3 OMP SIMD UNOPTIMIZED a.txt b.txt 5