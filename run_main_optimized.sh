#!/bin/sh
make clean
clear
make main
./main O3 MPI OMP SIMD UNOPTIMIZED
gnuplot gnuplot_settings.gnu