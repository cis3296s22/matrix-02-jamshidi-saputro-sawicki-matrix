#!/bin/sh
make clean
clear
make main_unoptimized
make main_mpi
mpiexec -np 4 ./main_mpi a.txt b.txt 5
./main_unoptimized OMP SIMD UNOPTIMIZED a.txt b.txt 5
gnuplot gnuplot_settings.gnu