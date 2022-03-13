#include "mpi.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/times.h>
#define min(x, y) ((x)<(y)?(x):(y))

#include "mat.h"

int main(int argc, char* argv[])
{
    int nrows, ncols;
    double *aa;	/* the A matrix */
    double *bb;	/* the B matrix */
    double *cc1;	/* A x B computed using the mpi code you write */
    double *cc2;	/* A x B computed using the conventional algorithm */
    int myid, numprocs;
    double starttime, endtime;
    MPI_Status status;

    /* insert other global variables here */
    int sender, anstype;

    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &numprocs);
    MPI_Comm_rank(MPI_COMM_WORLD, &myid);
    if (argc > 1) {
        nrows = atoi(argv[1]);
        ncols = nrows;
        double* buffer = malloc(sizeof(double) * ncols);

        aa = gen_matrix(nrows, ncols);
        bb = gen_matrix(ncols, nrows);

        if (myid == 0) {
            // Master Code goes here
            int numsent = 0;

            MPI_Bcast(bb, ncols, MPI_DOUBLE, 0, MPI_COMM_WORLD);
            for (int i = 0; i < min(numprocs - 1, nrows); i++){
                for (int j = 0; i < ncols; j++){
                    buffer[j] = aa[i * ncols + j];
                }
                MPI_Send(buffer, ncols, MPI_DOUBLE, i + 1, i + 1, MPI_COMM_WORLD);
                numsent++;
            }

            double *ans = malloc(sizeof(double) * ncols);
            cc1 = malloc(sizeof(double) * nrows * nrows);

            for (int i = 0; i < nrows; i++){
                MPI_Recv(&ans, 1, MPI_DOUBLE, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
                sender = status.MPI_SOURCE;
                anstype = status.MPI_TAG;

            }




            starttime = MPI_Wtime();
            /* Insert your master code here to store the product into cc1 */


            endtime = MPI_Wtime();
            printf("%f\n",(endtime - starttime));
            cc2  = malloc(sizeof(double) * nrows * nrows);
            mmult_nonvectorized(cc2, aa, nrows, ncols, bb, ncols, nrows);
            compare_matrices(cc2, cc1, nrows, nrows);
        } else {
            // Slave Code goes here
        }
    } else {
        fprintf(stderr, "Usage matrix_times_vector <size>\n");
    }
    MPI_Finalize();
    return 0;
}
