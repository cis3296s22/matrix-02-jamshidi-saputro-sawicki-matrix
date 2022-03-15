#!/bin/sh
make clean
clear
make main
./main O3 MPI OMP SIMD UNOPTIMIZED a.txt b.txt 5