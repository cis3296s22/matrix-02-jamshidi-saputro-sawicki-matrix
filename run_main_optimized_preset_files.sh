#!/bin/sh
make clean
clear
make main
make main_mpi
mpiexec -np 4 ./main_mpi O3 a.txt b.txt 5
./main O3 OMP SIMD UNOPTIMIZED a.txt b.txt 5