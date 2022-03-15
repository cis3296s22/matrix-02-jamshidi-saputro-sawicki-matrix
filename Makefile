PGMS=mmult_omp_timing matrix_times_vector hello test_mmult mxv_omp_mpi mmult_mpi_omp main main_unoptimized main_omp main_mpi

all:	${PGMS}

main:		mmult.o main.o mat.c mmult_omp.o
	gcc -o main -fopenmp -O3 mmult.o mmult_omp.o main.o mat.c

main_unoptimized:		mmult.o main.o mat.c mmult_omp.o
	gcc -o main_unoptimized -fopenmp mmult.o mmult_omp.o main.o mat.c

main_omp:		mmult_omp.o main.o mat.c
	gcc -o main_omp -fopenmp -O3 mmult_omp.o main.o mat.c

main_mpi_optimized:		main_mpi.o mat.c mmult.o
	mpicc -o main_mpi_optimized -O3 main_mpi.o mat.c mmult.o

#new
mmult_mpi:		mmult_mpi.o mat.c mmult.o
	mpicc -o mmult_mpi -O3 mmult_mpi.o mat.c mmult.o

mmult_mpi.o:	mmult_mpi.c
	mpicc -c mmult_mpi.c
#new

#main_mpi_optimized.o:	main_mpi.c
#	mpicc -c main_mpi.c

#main_mpi:		mat.o main_mpi.o mmult_omp.o
#	mpicc -o main_mpi -fopenmp -O3 main_mpi.o mmult_omp.o mat.o

#main_optimized:		mmult.o main.o mat.c
#	gcc -o -O3 main mmult.o main.o mat.c

main.o:	main.c
	gcc -c main.c

#main_mpi.o: main_mpi.c
#	mpicc -c -O3 main_mpi.c

mmult_mpi_omp:		mmult.o mmult_mpi_omp.o mat.c
	mpicc -o mmult_mpi_omp -fopenmp -O3 mmult.o mmult_mpi_omp.o mat.c

mmult_mpi_omp.o:	mmult_mpi_omp.c
	mpicc -c -fopenmp -O3 mmult_mpi_omp.c

main_mpi:		main_mpi.o mat.c mmult.o
	mpicc -o main_mpi main_mpi.o mat.c mmult.o

main_mpi.o:	main_mpi.c
	mpicc -c main_mpi.c

mmult_omp_timing:	mmult.o mmult_omp.o mmult_omp_timing.o mat.c
	gcc -o mmult -fopenmp -O3 mmult.o mmult_omp.o mmult_omp_timing.o mat.c -o mmult_omp_timing

mat.o:	mat.c
	gcc -c mat.c 

mmult.o:	mmult.c
	gcc -c mmult.c

mmult_omp.o:	mmult_omp.c
	gcc -c -O3 -fopenmp mmult_omp.c

mmult_omp_timing.o:	mmult_omp_timing.c
	gcc -c -O3 mmult_omp_timing.c

matrix_times_vector:	matrix_times_vector.c mat.c
	mpicc -O3 -o matrix_times_vector matrix_times_vector.c mat.c

hello:	hello.c
	mpicc -O3 -o hello hello.c

mxv_omp_mpi:	mxv_omp_mpi.c mat.c
	mpicc -fopenmp -O3 -o mxv_omp_mpi mxv_omp_mpi.c mat.c

test_mmult:	test_mmult.c mmult.c mat.c
	gcc test_mmult.c mmult.c mat.c -lm -o test_mmult

clean:
	rm -f *.o
	rm -f ${PGMS}

