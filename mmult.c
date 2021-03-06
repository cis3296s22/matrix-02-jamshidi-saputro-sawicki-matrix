/**
 * An unoptimized implementation of matrix multiplication.
 */

#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#include "mat.h"

int mmult_vectorized(double *c, double *a, double *b, int N) {
	int i, j, k, l;

	for (i = 0; i < N; i++) {
		for (j = 0; j < N; j++)
			c[i * N + j] = 0;

		for (k = 0; k < N; k++)
			for (l = 0; l < N; l++)
				c[i * N + l] += a[i * N + k] * b[k * N + l];
	}

	return 0;
}

int mmult_nonvectorized(double *c,
						double *a, int aRows, int aCols,
						double *b, int bRows, int bCols) {
	int i, j, k;

	for (i = 0; i < aRows; i++)
		for (j = 0; j < bCols; j++) {
			c[i * bCols + j] = 0;

			for (k = 0; k < aRows; k++)
				c[i * bCols + j] += a[i * aRows + k] * b[k * bCols + j];
		}

	return 0;
}