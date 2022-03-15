#!/bin/sh
make clean
clear
make main
./main O3 OMP SIMD UNOPTIMIZED
gnuplot gnuplot_settings.gnu