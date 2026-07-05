#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "solve.h"
#include "array_io.h"

int is_symmetric (double *a, int n)
{
	int i = 0, j = 0;
	for (i = 0; i < n; i ++)
		{
			for (j = 0; j < n; j ++)
				{
					if (!(a[i * n + j] <= a[j * n + i] && a[i * n + j] >= a[j * n + i])) return 0;
				}
		}
	return 1;
}

int matrix_trace (double *a, int n)
{
	int i = 0;
	double trace = 0;
	for (i = 0; i < n; i ++)
		{
			trace += a[i * n + i];
		}
	return trace;
}

void transposed_matrix (double *a, int n)
{
	int i = 0, j = 0;
	double tmp;
	for (i = 0; i < n; i ++)
		{
			for (j = i + 1; j < n; j ++)
				{
					tmp = a[i * n + j];
					a[i * n + j] = a[j * n + i];
					a[j * n + i] = tmp;
				}
		}
}

void symmetric_part (double *a, int n)
{
	int i = 0, j = 0;
	for (i = 0; i < n; i ++)
		{
			for (j = i + 1; j < n; j ++)
				{
                                        a[i * n + j] = (a[j * n + i] + a[i * n + j]) / 2.;
                                        a[j * n + i] = a[i * n + j];
				}
		}
}

void antisymmetric_part (double *a, int n)
{
	int i = 0, j = 0;
	for (i = 0; i < n; i ++)
		{
                        for (j = i; j < n; j ++)
				{
                                        a[i * n + j] = (a[j * n + i] - a[i * n + j]) / 2.;
                                        a[j * n + i] = -a[i * n + j];
                                }
		}
}

void el_row_operation_2 (double *a, int n, int m, int i,  int j)
{
	int k = 0;
	double tmp;
	(void) n;
	i --; j --;
	for (k = 0; k < m; k ++)
		{
				tmp = a[i * m + k];
				a[i * m + k] = a[j * m + k];
				a[j * m + k] = tmp;
		}
}

void el_column_operation_2 (double *a, int n, int m, int i,  int j)
{
	int k = 0;
	double tmp;
	(void) m;
	i --; j --;
	for (k = 0; k < n; k ++)
		{
				tmp = a[k * m + i];
				a[k * m + i] = a[k * m + j];
				a[k * m + j] = tmp;
		}
}

void el_row_operation_1 (double *a, int n, int m, int i,  int j, double g)
{
	int k = 0;
	(void) n;
	i --; j --;
	for (k = 0; k < m; k ++)
		{
				a[j * m + k] += g * a[i * m + k];
		}
}


void matrix_vector_multiplication (double *a, double *vec_b, int m, double *vec_c, int n)
{
	int i = 0, j = 0;
	for (i = 0; i < n; i++)
		{
                        vec_c[i] = 0;
			for (j = 0; j < m; j++) 
				{
					vec_c[i] += a[i * m + j] * vec_b[j]; 
				}	
		}
}

void matrix_multiplication (double *a, int n, int m, double *b, int k, double *c)
{
	int i = 0, j = 0, l = 0;
	for (i = 0; i < n; i++)
		{
			for (j = 0; j < k; j ++)
				{
                                        c[i * k + j] = 0;
					for (l = 0; l < m; l ++)
						{
							c[i * k + j] += a[i * m + l] * b[l * k + j];
						}
				}
		}
}





