#include <stdio.h>
#include <stdlib.h>
#include <sys/times.h>
#include <time.h>
#include <string.h>
#include "mat.h"
// #include "mpi.h"

int main(int argc, char *argv[]) {
	int matrix_size = 1;
	double *matrixA, *matrixB, *outputMatrix;
	struct timespec start, end;

	// Create output file or truncate it to length 0 if it exists
	FILE *output_fp = fopen ("output.txt", "w+");

	// Create header
	fprintf(output_fp, "matrix_size,elapsed_time\n");

	// Matrix size starts at 2
	while (++matrix_size <= 1000) {
		// Create matrix A, B and an empty output
		matrixA = gen_matrix(matrix_size, matrix_size);
		matrixB = gen_matrix(matrix_size, matrix_size);
		outputMatrix = malloc(sizeof(double) * matrix_size * matrix_size);

		clock_gettime(CLOCK_REALTIME, &start);
		mmult(outputMatrix,
			  matrixA, matrix_size, matrix_size,
			  matrixB, matrix_size, matrix_size);
		clock_gettime(CLOCK_REALTIME, &end);

		double elapsed_time = (end.tv_sec - start.tv_sec) + 1.0e-9 * (end.tv_nsec - start.tv_nsec);

		fprintf(output_fp, "%d,%f\n", matrix_size, elapsed_time);
		printf("\nCompleted matrix size %d with time %f", matrix_size, elapsed_time);

		free(matrixB);
		free(outputMatrix);
    }

	fclose(output_fp);
	printf("\nDone!");

	return 0;
}
