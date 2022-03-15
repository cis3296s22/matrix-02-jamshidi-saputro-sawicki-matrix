#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/times.h>
#include <time.h>

#include "mat.h"

// Static
#define MAX_N 1000

// Function declarations
double time_matrix(int *mode, double *output_matrix, double *matrix_a, double *matrix_b, int *N, struct timespec start, struct timespec end);
void interpret_args(int argc, char *argv[], int *OMP, int *SIMD, int *UNOPTIMIZED, int *OTHREE, int *cur_input_index, char *input_files[], int *N);

// void getUserSettings(int* SIMD, int* IOFile, int* OMP) {
// 	char str[100];
// 	printf("Would you like to use:\n 1. SIMD\n 2. OMP\n 3. None\n");
// 	fgets(str, 100, stdin);

// 	if (!strcmp(str, "1\n")) {
// 		*SIMD = 1;
// 	} else if (!strcmp(str, "2\n")) {
// 		*OMP = 1;
// 	}

// 	printf("Would you like to input your own matrices? (y/n): ");
// 	fgets(str, 100, stdin);

// 	if (!strcmp(str, "y\n")) {
// 		*IOFile = 1;
// 	}
// }

// double* getUserMatrix(int matrix_size) {
// 	char str[100];

// 	fgets(str, 100, stdin);
// 	str[strcspn(str, "\n")] = 0;
// 	double* matrix = read_matrix_from_file(str);

// 	print_matrix(matrix, matrix_size, matrix_size);
// 	printf("\n\n");

// 	return matrix;
// }

void record_time(int *mode, int *size, double elapsed_time, FILE *file) {
	fprintf(file, "%d,\t%f\n", *size, elapsed_time);
}

int main(int argc, char *argv[]) {
	int OMP = 0;
	int SIMD = 0;
	int UNOPTIMIZED = 0;
	int OTHREE = 0;
	int N = 2;
	int max_N = 1000;
	int cur_input_index = 0;
	char *input_files[2] = {'\0', '\0'};
	double *matrix_a, *matrix_b, *output_matrix;
	struct timespec start, end;

	// Output files
	char *omp_filename, *simd_filename, *unoptimized_filename;
	FILE *omp_file, *simd_file, *unoptimized_file;

	if (argc == 1) {
		printf("You must enter arguments.  Options: O3, OMP, SIMD, UNOPTIMIZED, filea.txt, fileb.txt, N");
	}

	interpret_args(argc, argv, &OMP, &SIMD, &UNOPTIMIZED, &OTHREE, &cur_input_index, input_files, &N);

	// If the O3 argument is present
	if (OTHREE) {
		omp_filename = "O3_omp.txt";
		simd_filename = "O3_simd.txt";
		unoptimized_filename = "03_unoptimized.txt";
	} else {
		omp_filename = "omp.txt";
		simd_filename = "simd.txt";
		unoptimized_filename = "unoptimized.txt";
	}

	// Create file descriptors
	omp_file = fopen(omp_filename, "w+");
	simd_file = fopen(simd_filename, "w+");
	unoptimized_file = fopen(unoptimized_filename, "w+");

	// Create the headers for every file
	fprintf(omp_file, "matrix_size,\telapsed_time\n");
	fprintf(simd_file, "matrix_size,\telapsed_time\n");
	fprintf(unoptimized_file, "matrix_size,\telapsed_time\n");

	// * Debugging prints.  Ignore if not debugging.
	// printf("\n%s, %s, %s", mpi_filename, omp_filename, simd_filename);
	// printf("\nMPI=%d\nOMP=%d\nSIMD=%d\nUNOPTIMIZED=%d\nFIRST_FILE=%s\nSECOND_FILE=%s\nN=%d", MPI, OMP, SIMD, UNOPTIMIZED, input_files[0], input_files[1], N);

	if (cur_input_index != 0) {	 // ! We're only doing files!
		if (input_files[0] == NULL || input_files[1] == NULL) {
			printf("\nYou must enter two matrix files along with a matrix size after both file names if you include extra parameters.\ne.g. 'a.txt b.txt 5'");
			return 1;
		}

		// Create matrices
		matrix_a = read_matrix_from_file(input_files[0]);
		matrix_b = read_matrix_from_file(input_files[1]);
		output_matrix = malloc(sizeof(double) * N * N);

		printf("\n------------------ MAIN ------------------");
		printf("\nMatrix %s:\n", input_files[0]);
		print_matrix(matrix_a, N, N);

		printf("\nMatrix %s:\n", input_files[1]);
		print_matrix(matrix_b, N, N);

		printf("\nRunning OMP...");
		if (OMP) {
			time_matrix(&OMP, output_matrix, matrix_a, matrix_b, &N, start, end);
			printf("\nResulting Matrix with OMP:\n");
			print_matrix(output_matrix, N, N);
		}

		printf("\nRunning SIMD...");
		if (SIMD) {
			time_matrix(&SIMD, output_matrix, matrix_a, matrix_b, &N, start, end);
			printf("\nResulting Matrix with SIMD:\n");
			print_matrix(output_matrix, N, N);
		}

		printf("\nRunning Unoptimized...");
		if (UNOPTIMIZED) {
			time_matrix(&UNOPTIMIZED, output_matrix, matrix_a, matrix_b, &N, start, end);
			printf("\nResulting Matrix with Unoptimized:\n");
			print_matrix(output_matrix, N, N);
		}

		free(matrix_a);
		free(matrix_b);
		free(output_matrix);
	} else {				   // ! Generating files from 0 to 1000!
		while (++N < MAX_N) {  // N starts at 3 because 2 + 1 = ++N at the first run
			matrix_a = gen_matrix(N, N);
			matrix_b = gen_matrix(N, N);
			output_matrix = malloc(sizeof(double) * N * N);

			printf("\n\nCurrent matrix size is now %d", N);

			printf("\nRunning OMP...");
			if (OMP) {
				record_time(&OMP, &N, time_matrix(&OMP, output_matrix, matrix_a, matrix_b, &N, start, end), omp_file);
			}

			printf("\nRunning SIMD...");
			if (SIMD) {
				record_time(&SIMD, &N, time_matrix(&SIMD, output_matrix, matrix_a, matrix_b, &N, start, end), simd_file);
			}

			printf("\nRunning Unoptimized...");
			if (UNOPTIMIZED) {
				record_time(&UNOPTIMIZED, &N, time_matrix(&UNOPTIMIZED, output_matrix, matrix_a, matrix_b, &N, start, end), unoptimized_file);
			}

			free(matrix_a);
			free(matrix_b);
			free(output_matrix);
		}
	}

	fclose(omp_file);
	fclose(simd_file);
	printf("\nDone!");

	return 0;
}

/**
 * @brief Overwrites all given args based on the parameters given when running this program
 *
 * @param argc
 * @param argv
 * @param OMP
 * @param SIMD
 * @param UNOPTIMIZED
 * @param OTHREE
 * @param cur_input_index
 * @param input_files
 * @param N
 */
void interpret_args(int argc, char *argv[], int *OMP, int *SIMD, int *UNOPTIMIZED, int *OTHREE, int *cur_input_index, char *input_files[], int *N) {
	for (int i = 1; i < argc; i++) {
		// printf("\nargv[%d]: %s", i, argv[i]);

		if (strcmp(argv[i], "OMP") == 0) {
			*OMP = 2;
		} else if (strcmp(argv[i], "SIMD") == 0) {
			*SIMD = 3;
		} else if (strcmp(argv[i], "UNOPTIMIZED") == 0) {
			*UNOPTIMIZED = 4;
		} else if (strcmp(argv[i], "O3") == 0) {
			*OTHREE = 1;
		} else {						  // Argument is not for run type, must be a file
			if (*cur_input_index == 2) {  // If we've already set two files in input_files
				*N = atoi(argv[i]);
			} else {
				input_files[(*cur_input_index)++] = argv[i];
			}

			// printf("\nCurrent file: %s with index %d", input_files[*cur_input_index], i);
		}
	}
}

/**
 * @brief
 *
 * @param mode 2 = OMP, 3 = SIMD, 4 = Unoptimized
 * @param output_matrix
 * @param matrix_a
 * @param matrix_b
 * @param N
 * @param start
 * @param end
 * @return double
 */
double time_matrix(int *mode, double *output_matrix, double *matrix_a, double *matrix_b, int *N, struct timespec start, struct timespec end) {
	clock_gettime(CLOCK_REALTIME, &start);

	switch (*mode) {
		case 2:	 // OMP
			mmult_omp(output_matrix, matrix_a, *N, *N, matrix_b, *N, *N);
			break;
		case 3:	 // SIMD
			mmult_vectorized(output_matrix, matrix_a, matrix_b, *N);
			break;
		case 4:	 // Unoptimized
			mmult_nonvectorized(output_matrix, matrix_a, *N, *N, matrix_b, *N, *N);
			break;
	}
	clock_gettime(CLOCK_REALTIME, &end);

	return (end.tv_sec - start.tv_sec) + 1.0e-9 * (end.tv_nsec - start.tv_nsec);
}