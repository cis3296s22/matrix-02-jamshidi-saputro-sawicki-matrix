#!/bin/sh
make clean
clear
make main_unoptimized
./main_unoptimized OMP SIMD UNOPTIMIZED a.txt b.txt 5
gnuplot gnuplot_settings.gnu