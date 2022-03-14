#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/times.h>
#include <time.h>

#include "mat.h"
// #include "mpi.h"

void getUserSettings(int* SIMD, int* IOFile, int* OMP){

    char str[100];
    printf("Would you like to use:\n 1. SIMD\n 2. OMP\n 3. None\n");
    fgets(str, 100, stdin);


    if (!strcmp(str, "1\n")){
        *SIMD = 1;
    }
    else if (!strcmp(str, "2\n")){
        *OMP = 1;
    }


    printf("Would you like to input your own matrices? (y/n): ");
    fgets(str, 100, stdin);

    if (!strcmp(str, "y\n")){
        *IOFile = 1;
    }
}

double* getUserMatrix(int matrix_size){

    char str[100];


    fgets(str, 100, stdin);
    str[strcspn(str, "\n")] = 0;
    double* matrix = read_matrix_from_file(str);

    print_matrix(matrix, matrix_size, matrix_size);
    printf("\n\n");

    return matrix;
}

int main(int argc, char *argv[]) {

    int SIMD = 0;
    int OMP = 0;
    int IOFile = 0;

    getUserSettings(&SIMD, &IOFile, &OMP);


	int matrix_size = 1;
	double *matrixA, *matrixB, *outputMatrix;
	struct timespec start, end;

	// Create output file or truncate it to length 0 if it exists
	FILE *output_fp = fopen("output.txt", "w+");

    if (IOFile){

        char str[100];
        printf("What is the size of the square matrices: ");
        fgets(str, 100, stdin);
        matrix_size = atoi(str);

        printf("Path of first matrix: ");
        matrixA = getUserMatrix(matrix_size);

        printf("Path of second matrix: ");
        matrixB = getUserMatrix(matrix_size);

        outputMatrix = malloc(sizeof(double) * matrix_size * matrix_size);


        if (SIMD){
            clock_gettime(CLOCK_REALTIME, &start);
            mmult_vectorized(outputMatrix,matrixA, matrixB, matrix_size);
            clock_gettime(CLOCK_REALTIME, &end);
        }
        else if (OMP){
            clock_gettime(CLOCK_REALTIME, &start);
            mmult_omp(outputMatrix, matrixA, matrix_size, matrix_size, matrixB, matrix_size, matrix_size);
            clock_gettime(CLOCK_REALTIME, &end);
        }
        else{
                clock_gettime(CLOCK_REALTIME, &start);
                mmult_nonvectorized(outputMatrix,
                                    matrixA, matrix_size, matrix_size,
                                    matrixB, matrix_size, matrix_size);
                clock_gettime(CLOCK_REALTIME, &end);
        }
        double elapsed_time = (end.tv_sec - start.tv_sec) + 1.0e-9 * (end.tv_nsec - start.tv_nsec);

        print_matrix(outputMatrix, matrix_size, matrix_size);
        printf("c^\nsize,time:\n%d,%f\n", matrix_size, elapsed_time);
        return 0;
    }

	// Create header
	fprintf(output_fp, "matrix_size,elapsed_time\n");

	// Matrix size starts at 2
	while (++matrix_size <= 1000) {
		// Create matrix A, B and an empty output
		matrixA = gen_matrix(matrix_size, matrix_size);
		matrixB = gen_matrix(matrix_size, matrix_size);
		outputMatrix = malloc(sizeof(double) * matrix_size * matrix_size);


        if (SIMD){
            clock_gettime(CLOCK_REALTIME, &start);
            mmult_vectorized(outputMatrix,matrixA, matrixB, matrix_size);
            clock_gettime(CLOCK_REALTIME, &end);
        }
        else if (OMP){
            clock_gettime(CLOCK_REALTIME, &start);
            mmult_omp(outputMatrix, matrixA, matrix_size, matrix_size, matrixB, matrix_size, matrix_size);
            clock_gettime(CLOCK_REALTIME, &end);
        }
        else{
            clock_gettime(CLOCK_REALTIME, &start);
            mmult_nonvectorized(outputMatrix,
                                matrixA, matrix_size, matrix_size,
                                matrixB, matrix_size, matrix_size);
            clock_gettime(CLOCK_REALTIME, &end);
        }


		double elapsed_time = (end.tv_sec - start.tv_sec) + 1.0e-9 * (end.tv_nsec - start.tv_nsec);

		fprintf(output_fp, "%d,%f\n", matrix_size, elapsed_time);
		printf("\nCompleted matrix size %d with time %f", matrix_size, elapsed_time);

        free(matrixA);
		free(matrixB);
		free(outputMatrix);
	}

	fclose(output_fp);
	printf("\nDone!");

	return 0;
}