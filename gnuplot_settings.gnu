set terminal png size 800,600 
set output 'out.png'

set key outside center right ## Legend placement

set title 'Matrix Multiplication Speeds on TU Wolfgang Cluster'
set xlabel 'Matrix Size'
set ylabel 'Time (Seconds)'

plot "O3_omp_1000.txt" every 1 using 1:2 title "-O3 OMP" with linespoints, \
	"omp_1000.txt" every 1 using 1:2 title "OMP" with linespoints