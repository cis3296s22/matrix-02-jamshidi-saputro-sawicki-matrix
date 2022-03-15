#!/bin/sh
make clean
clear
make main
make main_mpi_optimized
mpiexec -np 4 ./main_mpi_optimized O3 1000
./main O3 OMP SIMD UNOPTIMIZED
gnuplot gnuplot_settings.gnu