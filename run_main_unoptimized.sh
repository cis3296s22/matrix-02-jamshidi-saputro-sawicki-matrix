#!/bin/sh
make clean
clear
make main_unoptimized
./main_unoptimized OMP SIMD UNOPTIMIZED
gnuplot gnuplot_settings.gnu