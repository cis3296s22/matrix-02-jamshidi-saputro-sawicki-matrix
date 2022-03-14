set terminal png size 800,600 
set output 'out.png'

set key outside center right ## Legend placement

set title 'Matrix Multiplication Speeds on TU Wolfgang Cluster'
set xlabel 'Matrix Size'
set ylabel 'Time (Seconds)'

plot "O3_omp_1000.txt" using 1:2 title "OMP -O3" with linespoint, \
	"omp_1000.txt" using 1:2 title "OMP" with linespoint