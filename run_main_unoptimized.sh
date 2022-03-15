#!/bin/sh
make clean
clear
make main_unoptimized
make main_mpi
mpiexec -np 4 ./main_mpi 1000
./main_unoptimized OMP SIMD UNOPTIMIZED
gnuplot gnuplot_settings.gnu