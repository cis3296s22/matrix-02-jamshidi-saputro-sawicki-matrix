#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "mat.h"
#include "/usr/include/mpich/mpi.h"

int min(int a, int b){
    return a > b ? b : a;
}

int main(int argc, char* argv[])
{
    int matrix_size; // nrows & ncols
    double *matrixA, *matrixB, *outputMatrix, *compareMatrix, *buffer, *recv;
    int myid, numprocs;
    double starttime, endtime;
    MPI_Status status;

    /* insert other global variables here */

    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &numprocs);
    MPI_Comm_rank(MPI_COMM_WORLD, &myid);


    if (argc > 1) {
        matrix_size = atoi(argv[1]);
        buffer = malloc(sizeof(double) * matrix_size);

        if (myid == 0) {
            // MASTER CODE

            matrixA = gen_matrix(matrix_size, matrix_size);
            matrixB = gen_matrix(matrix_size, matrix_size);
            outputMatrix = malloc(sizeof(double) * matrix_size * matrix_size);
            recv = malloc(sizeof(double) * matrix_size);

            int numsent = 0;
            int sender, row;

            starttime = MPI_Wtime();

            // send all slaves matrixB (in the slides given, b was a vector but since we are multiplying by a matrix we have to give the entire matrixB)
            MPI_Bcast(matrixB, matrix_size * matrix_size, MPI_DOUBLE, 0, MPI_COMM_WORLD);
            for (int i = 0; i < min(numprocs - 1, matrix_size); i++){ // for each slave process or for the amount of rows
                for (int j = 0; j < matrix_size; j++){
                    buffer[j] = matrixA[i * matrix_size + j];
                }
                // send each slave process a row from matrixA (stored in buffer)
                MPI_Send(buffer, matrix_size, MPI_DOUBLE, i + 1, i + 1, MPI_COMM_WORLD); // i + 1 because slave processes start at 1
                numsent++;
            }

            for (int i = 0; i < matrix_size; i++) {
                // recv is a row with the answers filled in
                MPI_Recv(recv, matrix_size, MPI_DOUBLE, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
                sender = status.MPI_SOURCE;
                row = status.MPI_TAG;

                // input the answers to the outputMatrix based on the specific row(given by Recv)
                for (int j = 0; j < matrix_size; j++) {
                    outputMatrix[((row - 1) * matrix_size) + j] = recv[j];
                }

                if (numsent < matrix_size) {
                    // if there are more matrix rows in matrixA than the number of rows sent then we will send a new row after a slave process is done computing it's row
                    for (int j = 0; j < matrix_size; j++) {
                        buffer[j] = matrixA[numsent * matrix_size + j];
                    }

                    MPI_Send(buffer, matrix_size, MPI_DOUBLE, sender, numsent + 1, MPI_COMM_WORLD);
                    numsent++;
                } else {
                    // if there are no more rows to send then tell the slave processes to stop
                    MPI_Send(MPI_BOTTOM, 0, MPI_DOUBLE, sender, 0, MPI_COMM_WORLD);
                }
            }
            endtime = MPI_Wtime();


            printf("time: %f\n",(endtime - starttime));
            compareMatrix  = malloc(sizeof(double) * matrix_size * matrix_size);
            mmult_nonvectorized(compareMatrix, matrixA, matrix_size, matrix_size, matrixB, matrix_size, matrix_size);
            print_matrix(compareMatrix, matrix_size, matrix_size);
            printf("^compare\n\n");
            print_matrix(outputMatrix, matrix_size, matrix_size);
            printf("^output\n\n");
            compare_matrices(compareMatrix, outputMatrix, matrix_size, matrix_size);
        } else {
            // SLAVE CODE

            // Bcast will give us the values for matrixB
            matrixB = malloc(sizeof(double) * matrix_size * matrix_size);
            MPI_Bcast(matrixB, matrix_size * matrix_size, MPI_DOUBLE, 0, MPI_COMM_WORLD);
            if (myid <= matrix_size){ // if we have more processes than the size of the matrix then we don't want those to use those processes
                while (1){

                    // buffer holds a row from matrixA
                    MPI_Recv(buffer, matrix_size, MPI_DOUBLE, 0, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
                    if (status.MPI_TAG == 0){ // MPI_TAG will be 0 when we want to stop doing matrix multiplication
                        break;
                    }
                    int row = status.MPI_TAG;
                    double *ans = malloc(sizeof(double) * matrix_size);

                    // compute the answer by multiplying the row given from buffer by the matrixB column then send the answer back
                    for (int i = 0; i < matrix_size; i++){
                        for (int j = 0; j < matrix_size; j++){
                            ans[i] +=  buffer[j] * matrixB[j * matrix_size + i];
                        }
                    }
                    MPI_Send(ans, matrix_size, MPI_DOUBLE, 0, row, MPI_COMM_WORLD);
                }
            }
        }
    } else {
        fprintf(stderr, "Usage matrix_times_vector <size>\n");
    }
    MPI_Finalize();
    return 0;
}