set terminal png size 800,600 
set output 'out.png'

set key outside center right ## Legend placement

set title 'Matrix Multiplication Speeds on TU Wolfgang Cluster'
set xlabel 'Matrix Size'
set ylabel 'Time (Seconds)'
	
plot "O3_mpi.txt" every 1 using 1:2 title "-O3 MPI" with linespoints pointinterval 1001, \
	"mpi.txt" every 1 using 1:2 title "MPI" with linespoints pointinterval 1001, \
	"O3_omp.txt" every 1 using 1:2 title "-O3 OMP" with linespoints pointinterval 1001, \
	"omp.txt" every 1 using 1:2 title "OMP" with linespoints pointinterval 1001, \
	"O3_simd.txt" every 1 using 1:2 title "-O3 SIMD" with linespoints pointinterval 1001, \
	"simd.txt" every 1 using 1:2 title "SIMD" with linespoints pointinterval 1001, \
	"O3_unoptimized.txt" every 1 using 1:2 title "-O3 Non-Vectorized" with linespoints pointinterval 1001, \
	"unoptimized.txt" every 1 using 1:2 title "Non-Vectorized" with linespoints pointinterval 1001