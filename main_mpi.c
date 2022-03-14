#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "mat.h"
#include "mpi.h"

int main(int argc, char *argv[]) {
	MPI_Status status;	// MPI Status container

	int matrix_size = 5;
	int process_id;		// Set from mpiexec
	int process_count;	// Set when running mpiexec, e.g. mpiexec -n 4
	int slave_count;	// Determines the amount of slave processes we have.  = process_count - 1
	int rows;			// Determines the number of rows of a matrix that is sent to a slave process
	int offset;			// Determines the starting point of the row which is sent to a slave process
	int source;
	int destination;  // Determines the destination slave process ID with bonds of [1, slave_count]
	double *matrixA, *matrixB, *output_matrix;

	MPI_Init(&argc, &argv);
	MPI_Comm_size(MPI_COMM_WORLD, &process_count);
	MPI_Comm_rank(MPI_COMM_WORLD, &process_id);

	slave_count = process_count - 1;

	// printf("procs: %d and id: %d\n", process_count, process_id);

	if (process_id == 0) {	// Parent/Master/Controller process
		// Create matrices
		matrixA = gen_matrix(matrix_size, matrix_size);
		matrixB = gen_matrix(matrix_size, matrix_size);
		output_matrix = malloc(sizeof(double) * matrix_size * matrix_size);

		rows = matrix_size / slave_count;  // ! Determine number of rows of the Matrix A, that is sent to each slave process
		offset = 0;						   // ! Offset variable determines the starting point of the row which sent to slave process

		// ! Calculation details are assigned to slave tasks. Process 1 onwards; Each message's tag is 1
		for (destination = 1; destination <= slave_count; destination++) {
			MPI_Send(&offset, 1, MPI_INT, destination, 1, MPI_COMM_WORLD);											   // ! Acknowledging the offset of the Matrix A
			MPI_Send(&rows, 1, MPI_INT, destination, 1, MPI_COMM_WORLD);											   // ! Acknowledging the number of rows
			MPI_Send(&matrixA[offset * matrix_size], rows * matrix_size, MPI_DOUBLE, destination, 1, MPI_COMM_WORLD);  // ! Send rows of the Matrix A which will be assigned to slave process to compute
			MPI_Send(&matrixB, matrix_size * matrix_size, MPI_DOUBLE, destination, 1, MPI_COMM_WORLD);				   // ! Offset is modified according to number of rows sent to each process

			offset += rows;
		}

		for (int i = 1; i <= slave_count; i++) {
			source = i;

			MPI_Recv(&offset, 1, MPI_INT, source, 2, MPI_COMM_WORLD, &status);
			MPI_Recv(&rows, 1, MPI_INT, source, 2, MPI_COMM_WORLD, &status);
			MPI_Recv(&output_matrix[offset * matrix_size], rows * matrix_size, MPI_DOUBLE, source, 2, MPI_COMM_WORLD, &status);
		}

		// Print the result matrix
		printf("\nResult Matrix C = Matrix A * Matrix B:\n\n");
		// print_matrix(output_matrix, matrix_size, matrix_size);
		
	} else {		 // Child/Slave process
		source = 0;	 // ! Source process ID is defined

		MPI_Recv(&offset, 1, MPI_INT, source, 1, MPI_COMM_WORLD, &status);
		MPI_Recv(&rows, 1, MPI_INT, source, 1, MPI_COMM_WORLD, &status);
		MPI_Recv(&matrixA, rows * matrix_size, MPI_DOUBLE, source, 1, MPI_COMM_WORLD, &status);
		MPI_Recv(&matrixB, matrix_size * matrix_size, MPI_DOUBLE, source, 1, MPI_COMM_WORLD, &status);

		// !Multiplied vectors get sent back to the controller process
		MPI_Send(&offset, 1, MPI_INT, 0, 2, MPI_COMM_WORLD);							 // !Offset will be sent to Root, which determines the starting point of the calculated value in matrix C
		MPI_Send(&rows, 1, MPI_INT, 0, 2, MPI_COMM_WORLD);								 // ! Number of rows the process calculated will be sent to root process
		MPI_Send(&output_matrix, rows * matrix_size, MPI_DOUBLE, 0, 2, MPI_COMM_WORLD);	 // !Resulting matrix with calculated rows will be sent to root process
	}

	// printf("\nMatrix A:\n");
	// print_matrix(matrixA, matrix_size, matrix_size);

	// printf("\n\nMatrix B:\n");
	// print_matrix(matrixB, matrix_size, matrix_size);

	MPI_Finalize();
	return 0;
}