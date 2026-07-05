#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include "solve.h"

double r1_solver (double *a, double *x, double *b, int n)
{
	int i = 0, j = 0;
	double numerator = 0, denominator = 0, sum = 0;
	for (i = 0; i < n; i ++)
		{
			sum = 0;
			for (j = 0; j < n; j ++)
				{
					sum += a[i * n + j] * x[j];
				}
			numerator += fabs(sum - b[i]);
			denominator += fabs(b[i]);
		}
	return numerator / denominator;
}

double r2_solver (double *x, int n)
{
	int i = 0;
	double sum1 = 0, sum2 = 0;
	for (i = 0; i < n; i ++)
		{
			sum1 += fabs(x[i] - ((i + 1) % 2));
			sum2 += ((i + 1) % 2);
		}

	if (sum2 <= 0 && sum2 >= 0)
		{
			return 0;
		}
	return sum1 / sum2;
}

void compute_vector_b (double *a, double *b, int n)
{
	int i = 0, k = 0;
	for (i = 0; i < n; i ++)
		{
			b[i] = 0;
			for (k = 0; k <= (n - 1) / 2; k++)
				{
					b[i] += a[i * n + 2 * k];
				}
		}
}

void matrix_vector_multiplication (double *a, double *vec_b, int m, double *vec_c, int n)
{
	int i = 0, j = 0;
	double tmp = 0;
	for (i = 0; i < n; i++)
		{
			tmp = 0;
			for (j = 0; j < m; j++) 
				{
					tmp += a[i * m + j] * vec_b[j]; 
				}
			vec_c[i] = tmp;	
		}
}

double scalar_product (double *a, double *b, double n)
{
	int i = 0;
	double res = 0;
	for (i = 0; i < n; i ++) res += a[i] * b[i];
	return res;
}

void vector_subtract(double *a, double *b, double *result, int n) 
{
	int i = 0;
    for (i = 0; i < n; i++) result[i] = a[i] - b[i];
}

void solve_lower_triangular(double *a, double *rhs, double *sol, int n) 
{
    for (int i = 0; i < n; i++) 
	    {
	        double sum = 0.0;
	        for (int j = 0; j < i; j++) 
		        {
		            sum += a[i * n + j] * sol[j];
		        }
	        sol[i] = (rhs[i] - sum) / a[i * n + i];
	    }
}

void solve_upper_triangular(double *a, double *rhs, double *sol, int n) 
{
    for (int i = n - 1; i >= 0 ; i--) 
	    {
	        double sum = 0.0;
	        for (int j = i + 1; j < n; j++) 
		        {
		            sum += a[i * n + j] * sol[j];
		        }
	        sol[i] = (rhs[i] - sum) / a[i * n + i];
	    }
}

void solve_symmetric_system(double *a, double *b, double *w, int n) {
    solve_lower_triangular (a, b, w, n);
    for (int i = 0; i < n; i++) {
        w[i] /= a[i * n + i];
    }
    solve_upper_triangular (a, b, w, n);
}

double r1task01 (double *a, double *x0, double *x, int n, int m)
{
	int i = 0, j = 0;
	double numerator = 0, denominator = 0;

	matrix_vector_multiplication(a, x0, n, x, n);

	for (i = 1; i < m; i ++)
		{
			for (j = 0; j < n; j ++)
				{
					x0[j] = x[j];
				}
			matrix_vector_multiplication(a, x0, n, x, n);
		}

	for (j = 0; j < n; j ++)
		{
			x0[j] = x[j];
		}
	matrix_vector_multiplication(a, x0, n, x, n);


	numerator = 0;
	denominator = 0;

	for (i = 0; i < n; i ++)
		{
			numerator += x[i] * x0[i];
			denominator += x0[i] * x0[i];
		}

	if (fabs(denominator) < EPS)
		{
			return -1;
		}
	for (j = 0; j < n; j ++)
		{
			double tmp = x[j];
			x[j] = x0[j];
			x0[j] = tmp;
		}
	return numerator / denominator;
}

double r2task01 (double *a, double *x, int n, double r1)
{
	int i = 0, j = 0;
	double numerator = 0, denominator = 0, sum = 0;
	for (i = 0; i < n; i ++)
		{
			sum = 0;
			for (j = 0; j < n; j ++)
				{
					sum += a[i * n + j] * x[j];
				}
			numerator += fabs(sum - r1 * x[i]);
			denominator += fabs(r1 * x[i]);
		}
	if (fabs(denominator) < EPS)
		{
			return -1;
		}
	return numerator / denominator;
}

task_status task02 (double *a, double *x0, double *x, double *b, int n, int m, double t)
{
	int i = 0, j = 0;
	for (i = 0; i < m; i ++) 
		{
	        matrix_vector_multiplication(a, x0, n, x, n);

	        for (j = 0; j < n; j ++)
	        	{
	        		x[j] = x0[j] - t * (x[j] - b[j]);
	        	}

	        for (j = 0; j < n; j++) 
		        {
		            x0[j] = x[j];
		        }
	    }
	return SUCCESS_;
}

task_status task03 (double *a, double *x0, double *x, double *b, double *r, int n, int m)
{
	int i = 0, j = 0;
	double t = 0, numerator = 0, denominator = 0, tmp = 0;

	matrix_vector_multiplication(a, x0, n, x, n);
	vector_subtract(x, b, r, n);

	for (j = 0; j < n; j++) 
        {
            x[j] = x0[j];
        }


	for (i = 0; i < m; i ++)
		{	
			matrix_vector_multiplication (a, r, n, x0, n);
			numerator = scalar_product (r, r, n);
			denominator = scalar_product (x0, r, n);

			for (j = 0; j < n; j ++)
				{
					tmp = r[j];
					numerator += tmp * tmp;
					denominator += x0[j] * tmp;
				}

			if (fabs(denominator) < EPS) break;
			t = numerator / denominator;

			for (j = 0; j < n; j ++)
				{
					x[j] -= t * r[j];
					r[j] -= t * x0[j]; 
					x0[j] = x[j];
				}
		}
	return SUCCESS_;
}

task_status task04 (double *a, double *x0, double *x, double *b, double *r, int n, int m)
{
	int i = 0, j = 0;
	double t = 0, numerator = 0, denominator = 0;

	matrix_vector_multiplication(a, x0, n, x, n);
	vector_subtract(x, b, r, n);

	for (j = 0; j < n; j++) 
        {
            x[j] = x0[j];
        }


	for (i = 0; i < m; i ++)
		{	
			matrix_vector_multiplication (a, r, n, x0, n);
			numerator = scalar_product (x0, r, n);
			denominator = scalar_product (x0, x0, n);
			if (fabs(denominator) < EPS) break;
			t = numerator / denominator;

			for (j = 0; j < n; j ++)
				{
					x[j] -= t * r[j];
					r[j] -= t * x0[j]; 
					x0[j] = x[j];
				}
		}
		return SUCCESS_;
}

task_status task05 (double *a, double *x0, double *x, double *b, double *r, int n, int m)
{
	int i = 0, j = 0;
	double t = 0, numerator = 0, denominator = 0;

	for (i = 0; i < n; i ++)
		{
			if (fabs(a[i * n + i]) < EPS) return ERROR_MATRIX;
		}

	matrix_vector_multiplication(a, x0, n, r, n);
	vector_subtract(r, b, r, n);

	for (j = 0; j < n; j++) 
        {
            x[j] = x0[j];
        }

	for (i = 0; i < m; i ++)
		{	
			for (j = 0; j < n; j ++)
				{
					x0[j] = r[j] / a[j * n + j];
				}
			numerator = scalar_product (x0, r, n);
			matrix_vector_multiplication (a, x0, n, r, n);
			denominator = scalar_product (x0, r, n);
			if (fabs(denominator) < EPS) break;
			t = numerator / denominator;
			for (j = 0; j < n; j ++)
				{
					x[j] -= t * x0[j];
					r[j] = x0[j] * a[j * n + j] - t * r[j];
					x0[j] = x[j];
				}
		}
		return SUCCESS_;
}

task_status task06 (double *a, double *x0, double *x, double *b, double *r, int n, int m)
{
	int i = 0, j = 0;
	double t = 0, numerator = 0, denominator = 0;

	for (i = 0; i < n; i ++)
		{
			if (fabs(a[i * n + i]) < EPS) return ERROR_MATRIX;
		}

	matrix_vector_multiplication(a, x0, n, r, n);
	vector_subtract(r, b, r, n);

	for (j = 0; j < n; j++) 
        {
            x[j] = x0[j];
        }

	for (i = 0; i < m; i ++)
		{	
			for (j = 0; j < n; j ++)
				{
					x0[j] = r[j] / a[j * n + j];
				}
			matrix_vector_multiplication (a, x0, n, r, n);
			numerator = 0;
			for (j = 0; j < n; j ++)
				{
					numerator += x0[j] * a[j * n + j] * r[j];
				}
			denominator = scalar_product (r, r, n);
			if (fabs(denominator) < EPS) break;
			t = numerator / denominator;
			for (j = 0; j < n; j ++)
				{
					x[j] -= t * x0[j];
					r[j] = x0[j] * a[j * n + j] - t * r[j];
					x0[j] = x[j];
				}
		}
		return SUCCESS_;
}

task_status task07 (double *a, double *x0, double *x, double *b, double *r, int n, int m, double t)
{
	int i = 0, j = 0;

	for (i = 0; i < n; i ++)
		{
			if (fabs(a[i * n + i]) < EPS) return ERROR_MATRIX;
		}

	matrix_vector_multiplication(a, x0, n, r, n);
	vector_subtract(r, b, r, n);

	for (j = 0; j < n; j++) 
        {
            x[j] = x0[j];
        }

	for (i = 0; i < m; i ++)
		{	
			for (j = 0; j < n; j ++)
				{
					x0[j] = r[j] / a[j * n + j];
				}
			matrix_vector_multiplication (a, x0, n, r, n);
			for (j = 0; j < n; j ++)
				{
					x[j] -= t * x0[j];
					r[j] = x0[j] * a[j * n + j] - t * r[j];
					x0[j] = x[j];
				}
		}
	return SUCCESS_;
}

task_status task08 (double *a, double *x0, double *x, double *b, double *r, double *w, int n, int m, double t)
{
	int i = 0, j = 0;

	for (i = 0; i < n; i ++)
		{
			if (fabs(a[i * n + i]) < EPS) return ERROR_MATRIX;
		}

	matrix_vector_multiplication(a, x0, n, r, n);
	vector_subtract(r, b, r, n);

	for (j = 0; j < n; j++) 
        {
            x[j] = x0[j];
        }

	for (i = 0; i < m; i ++)
		{	
			matrix_vector_multiplication (a, x0, n, r, n);
			for (j = 0; j < n; j ++)
				{
					r[j] = t * (b[j] - r[j]);
				}
			solve_lower_triangular (a, r, w, n);
			for (j = 0; j < n; j ++)
				{
					x0[j] += w[j];
					x[j] = x0[j];
				}
		}
	return SUCCESS_;
}

task_status task09 (double *a, double *x0, double *x, double *b, double *r, double *w, int n, int m, double t)
{
	int i = 0, j = 0;

	for (i = 0; i < n; i ++)
		{
			if (fabs(a[i * n + i]) < EPS) return ERROR_MATRIX;
		}

	matrix_vector_multiplication(a, x0, n, r, n);
	vector_subtract(r, b, r, n);

	for (j = 0; j < n; j++) 
        {
            x[j] = x0[j];
        }

	for (i = 0; i < m; i ++)
		{	
			matrix_vector_multiplication (a, x0, n, r, n);
			for (j = 0; j < n; j ++)
				{
					r[j] = t * (b[j] - r[j]);
				}
			solve_upper_triangular (a, r, w, n);
			for (j = 0; j < n; j ++)
				{
					x0[j] += w[j];
					x[j] = x0[j];
				}
		}
		return SUCCESS_;
}

task_status task10 (double *a, double *x0, double *x, double *b, double *r, double *w, int n, int m, double t)
{
	int i = 0, j = 0;

	for (i = 0; i < n; i ++)
		{
			if (fabs(a[i * n + i]) < EPS) return ERROR_MATRIX;
		}

	matrix_vector_multiplication(a, x0, n, r, n);
	vector_subtract(r, b, r, n);

	for (j = 0; j < n; j++) 
        {
            x[j] = x0[j];
        }

	for (i = 0; i < m; i ++)
		{	
			matrix_vector_multiplication (a, x0, n, r, n);
			for (j = 0; j < n; j ++)
				{
					r[j] = t * (b[j] - r[j]);
				}
			solve_symmetric_system (a, r, w, n);
			for (j = 0; j < n; j ++)
				{
					x0[j] += w[j];
					x[j] = x0[j];
				}
		}
	return SUCCESS_;
}

