#ifndef MAT_H
#define MAT_H

double *gen_matrix(int n, int m);
int compare_matrices(double *a, double *b, int nRows, int nCols);
double *read_matrix_from_file(const char *path);
void print_matrix(double *a, int nrows, int ncols);

/**
 * An optimized algorithm for matrix multiplication.
 *
 * @param c : the matrix in which to place the result of the matrix multiplication.
 * @param a : the first matrix.
 * @param b : the second matrix.
 * @param N : the number of rows and columns in the given NxN matrices
 *
 * @return 0 if the matrix multiplication is successful.
 */
int mmult_vectorized(double *c, double *a, double *b, int N);

/**
 * An unoptimized algorithm for matrix multiplication.
 *
 * @param c : the matrix in which to place the result of the matrix multiplication.
 * @param a : the first matrix.
 * @param aRows : the number of rows in a.
 * @param aCols : the number of columns in a.
 * @param b : the second matrix.
 * @param bRows : the number of rows in b.
 * @param bCols : the number of columns in b.
 * @return 0 if the matrix multiplication is successful.
 */
int mmult_nonvectorized(double *c,
						double *a, int aRows, int aCols,
						double *b, int bRows, int bCols);

/**
 * An algorithm for matrix multiplication using OMP.
 *
 * @param c : the matrix in which to place the result of the matrix multiplication.
 * @param a : the first matrix.
 * @param aRows : the number of rows in a.
 * @param aCols : the number of columns in a.
 * @param b : the second matrix.
 * @param bRows : the number of rows in b.
 * @param bCols : the number of columns in b.
 * @return 0 if the matrix multiplication is successful.
 */
int mmult_omp(double *c,
              double *a, int aRows, int aCols,
              double *b, int bRows, int bCols);

/**
 * An algorithm for matrix multiplication using OMP.
 *
 * @param c : the matrix in which to place the result of the matrix multiplication.
 * @param a : the first matrix.
 * @param aRows : the number of rows in a.
 * @param aCols : the number of columns in a.
 * @param b : the second matrix.
 * @param bRows : the number of rows in b.
 * @param bCols : the number of columns in b.
 * @return 0 if the matrix multiplication is successful.
 */
int mmult_mpi(double *c,
              double *a, int aRows, int aCols,
              double *b, int bRows, int bCols);

#endif