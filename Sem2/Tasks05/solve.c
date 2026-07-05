#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include "solve.h"
#include "array_io.h"

double max_row_sum (double *a, int n, int m)
{
	int i = 0, j = 0;
	double sum = 0, mx_sum = 0;
	for (i = 0; i < n; i ++)
		{
			sum = 0;
			for (j = 0; j < m; j ++)
				{
					sum += fabs(a[i * m + j]);
				}
			if (sum > mx_sum) mx_sum = sum;
		}
	return mx_sum;
}


double max_column_sum (double *a, int n, int m)
{
	int i = 0, j = 0;
	double sum = 0, mx_sum = 0;
	for (j = 0; j < m; j ++)
		{
			sum = 0;
			for (i = 0; i < n; i ++)
				{
					sum += fabs(a[i * m + j]);
				}
			if (sum > mx_sum) mx_sum = sum;
		}
	return mx_sum;
}

double residual_vector_sum (double *a, double *vec_x, double *vec_b, int n, int m)
{
	int i = 0, j = 0;
	double tmp = 0;
	double r_sum = 0;
	for (i = 0; i < n; i++)
		{
			tmp = 0;
			for (j = 0; j < m; j++) 
				{
					tmp += a[i * m + j] * vec_x[j]; 
				}
			r_sum += fabs(tmp - vec_b[i]);	
		}
	return r_sum;
}

double residual_vector_max (double *a, double *vec_x, double *vec_b, int n, int m)
{
	int i = 0, j = 0;
	double tmp = 0;
	double mx = 0;
	for (i = 0; i < n; i++)
		{
			tmp = 0;
			for (j = 0; j < m; j++) 
				{
					tmp += a[i * m + j] * vec_x[j]; 
				}
			tmp -= vec_b[i];
			if (tmp > mx)
				{
					mx = tmp;
				}	
		}
	return mx;
}

double residual_matrix_row_sum (double *a, double *b, int n, int m)
{
	int i = 0, j = 0, l = 0;
	double tmp = 0;
	double mx_sum = 0, row_sum = 0;

	for (i = 0; i < n; i++)
		{
			row_sum = 0;
			for (j = 0; j < n; j ++)
				{
					tmp = 0;
					for (l = 0; l < m; l ++)
						{
							tmp += a[i * m + l] * b[l * n + j];
						}
					if (i == j) tmp--;
					row_sum += fabs(tmp);
				}
			if (row_sum > mx_sum) mx_sum = row_sum;
		}
	return mx_sum;
}

double residual_matrix_column_sum (double *a, double *b, int n, int m)
{
	int i = 0, j = 0, l = 0;
	double tmp = 0;
	double mx_sum = 0, column_sum = 0;

	for (j = 0; j < n; j ++)
		{
			column_sum = 0;
			for (i = 0; i < n; i ++)
				{
					tmp = 0;
					for (l = 0; l < m; l ++)
						{
							tmp += a[i * m + l] * b[l * n + j];
						}
					if (i == j) tmp--;
					column_sum += fabs(tmp);
				}
			if (column_sum > mx_sum) mx_sum = column_sum;
		}
	return mx_sum;
}

double compute_laplacian_1 (double *a, int n, int m)
{
	int i = 0, j = 0;
	double mx = 0;
	double tmp = 0;

	for (i = 1; i < n - 1; i ++)
		{
			for (j = 1; j < m - 1; j ++)
				{
					tmp = (a[(i - 1) * m + j] + a[(i + 1) * m + j] + a[i * m + j - 1] + a[i * m + j + 1]) * 0.2;
					a[i * m + j] = tmp;
                    if (fabs(tmp) > mx) mx = fabs(tmp);
				}
		}

	for (j = 0; j < m; j ++)
		{
			if (fabs(a[j]) > mx) mx = fabs(a[j]);
			if (fabs(a[(n - 1) * m + j]) > mx) mx = fabs(a[(n - 1) * m + j]);
		}

	for (i = 1; i < n - 1; i ++)
		{
			if (fabs(a[i * m]) > mx) mx = fabs(a[i * m]);
			if (fabs(a[i * m + m - 1]) > mx) mx = fabs(a[i * m + m - 1]);
		}

	return mx;
}

double compute_laplacian_2 (double *a, int n, int m)
{
	int i = 0, j = 0;
	double mx = 0;
	double tmp = 0;

	for (i = n - 2; i > 0; i --)
		{
			for (j = m - 2; j > 0; j --)
				{
					tmp = (a[(i - 1) * m + j] + a[(i + 1) * m + j] + a[i * m + j - 1] + a[i * m + j + 1]) * 0.2;
					a[i * m + j] = tmp;
                    if (fabs(tmp) > mx) mx = fabs(tmp);
				}
		}

	for (j = 0; j < m; j ++)
		{
			if (fabs(a[j]) > mx) mx = fabs(a[j]);
			if (fabs(a[(n - 1) * m + j]) > mx) mx = fabs(a[(n - 1) * m + j]);
		}

	for (i = 1; i < n - 1; i ++)
		{
			if (fabs(a[i * m]) > mx) mx = fabs(a[i * m]);
			if (fabs(a[i * m + m - 1]) > mx) mx = fabs(a[i * m + m - 1]);
		}

	return mx;
}

double compute_laplacian_3 (double *a, int n, int m)
{
	int i = 0, j = 0;
	double mx = 0;
	double tmp = 0;

	for (i = 1; i < n - 1; i ++)
		{
			for (j = m - 2; j > 0; j --)
				{
					tmp = (a[(i - 1) * m + j] + a[(i + 1) * m + j] + a[i * m + j - 1] + a[i * m + j + 1]) * 0.2;
					a[i * m + j] = tmp;
                	if (fabs(tmp) > mx) mx = fabs(tmp);
				}
		}

	for (j = 0; j < m; j ++)
		{
			if (fabs(a[j]) > mx) mx = fabs(a[j]);
			if (fabs(a[(n - 1) * m + j]) > mx) mx = fabs(a[(n - 1) * m + j]);
		}

	for (i = 1; i < n - 1; i ++)
		{
			if (fabs(a[i * m]) > mx) mx = fabs(a[i * m]);
			if (fabs(a[i * m + m - 1]) > mx) mx = fabs(a[i * m + m - 1]);
		}

	return mx;
}

double compute_laplacian_4 (double *a, int n, int m)
{
	int i = 0, j = 0;
	double mx = 0;
	double tmp = 0;

	for (i = n - 2; i > 0; i --)
		{
			for (j = 1; j < m - 1; j ++)
				{
					tmp = (a[(i - 1) * m + j] + a[(i + 1) * m + j] + a[i * m + j - 1] + a[i * m + j + 1]) * 0.2;
					a[i * m + j] = tmp;
                    if (fabs(tmp) > mx) mx = fabs(tmp);
				}
		}

	for (j = 0; j < m; j ++)
		{
			if (fabs(a[j]) > mx) mx = fabs(a[j]);
			if (fabs(a[(n - 1) * m + j]) > mx) mx = fabs(a[(n - 1) * m + j]);
		}

	for (i = 1; i < n - 1; i ++)
		{
			if (fabs(a[i * m]) > mx) mx = fabs(a[i * m]);
			if (fabs(a[i * m + m - 1]) > mx) mx = fabs(a[i * m + m - 1]);
		}

	return mx;
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

double mx_to_a11 (double *a, int n, int m)
{
	int i = 0, j = 0, mx_i = 0, mx_j = 0;
	double mx = 0;

	for (i = 0; i < n; i ++)
		{
			for (j = 0; j < m; j ++)
				{
					if (fabs(a[i * m + j]) > mx)
						{
							mx_i = i;
							mx_j = j;
							mx = fabs(a[i * m + j]);
						}
				}
		}


	el_row_operation_2 (a, n, m, 1, mx_i + 1);
	el_column_operation_2 (a, n, m, 1, mx_j + 1);

	return mx;
}









