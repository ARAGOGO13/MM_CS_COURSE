#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include "solve.h"
#include <time.h>




int is_symmetric (double *a, int n)
{
	int i = 0, j = 0;
	for (i = 0; i < n; i ++)
		{
			for (j = i; j < n; j ++)
				{
					if (!(a[i * n + j] <= a[j * n + i] && a[i * n + j] >= a[j * n + i])) return 0;
				}
		}
	return 1;
}

double r_solver (double *a, double *a_inv, int n, double *r1, double *r2)
{
	int i = 0, j = 0, k = 0;
	double sum_r1 = 0, sum_r2 = 0, column_sum_r1 = 0, column_sum_r2 = 0, mx_sum_r1 = 0, mx_sum_r2 = 0;


	if (n > 4000) return 0;

	for (j = 0; j < n; j ++)
        {
            column_sum_r1 = 0;
            column_sum_r2 = 0;
            for (i = 0; i < n; i ++)
                {
                    sum_r1 = 0;
                    sum_r2 = 0;
                    for (k = 0; k < n; k ++)
                        {	
                            sum_r1 += a[IJ_EL(i, k)] * a_inv[IJ_EL(k, j)];
                            sum_r2 += a_inv[IJ_EL(i, k)] * a[IJ_EL(k, j)];
                        }
                    if (i == j) 
                        {
                            sum_r1 --;
                            sum_r2 --;
                        }
                    column_sum_r1 += fabs(sum_r1);
                    column_sum_r2 += fabs(sum_r2);
                }
            if (column_sum_r1 > mx_sum_r1) mx_sum_r1 = column_sum_r1;
            if (column_sum_r2 > mx_sum_r2) mx_sum_r2 = column_sum_r2; 
		}
	*r1 = mx_sum_r1;
	*r2 = mx_sum_r2;
	return 1;
}

double matrix_norm (double *A, int n)
{
    double sum = 0, mx = 0;
    int i = 0, j = 0;
    for (i = 0; i < n; i ++)
        {
            sum = 0;
            for (j = 0; j < n; j ++)
                {
                    sum += fabs(A[IJ_EL(i, j)]);
                }
            if (sum > mx) mx = sum;
        }
    return mx;
}

int cholesky_inverse(int n, double* A, double* R, double* D) 
{
    int i, j, k;
    double norm = matrix_norm(A, n);
 
    for(i = 0; i < n; i++)
        {
            double temp = A[IJ_EL(i, i)];
            for(k = 0; k < i; k++)
                {
                    temp -= A[IJ_EL(i, k)] * A[IJ_EL(i, k)] * D[k];
                }
            D[i] = (temp < 0 ? -1 : 1);
            temp = fabs(temp);
            if(temp <= EPS * norm) return -1;
            temp = sqrt(temp);
            A[IJ_EL(i, i)] = temp;
            temp = D[i] / temp;
            for(j = i + 1; j < n; j++)
                {
                    double s = A[IJ_EL(i, j)];
                    for(k = 0; k < i; k++)
                        {
                            s -= A[IJ_EL(i, k)] * D[k] * A[IJ_EL(j, k)];
                        }
                    A[IJ_EL(j, i)] = s * temp;
                }
        }

 
    for(i = 0; i < n; i++)
        {
            for(j = 0; j < i; j++)
                {
                    A[IJ_EL(j, i)] = A[IJ_EL(i, j)];
                }
        }
    
    for(i = n - 1; i >= 0; i--)
        {
            double temp = 1.0 / A[IJ_EL(i, i)];
            A[IJ_EL(i, i)] = temp;
            for(j = i + 1; j < n; j++)
                {
                    A[IJ_EL(i, j)] *= temp;
                }
            for(k = 0; k < i; k++)
                {
                    double mult = A[IJ_EL(k, i)];
                    A[IJ_EL(k, i)] = -(mult * temp);
                    for(j = i + 1; j < n; j++)
                        {
                            A[IJ_EL(k, j)] -= mult * A[IJ_EL(i, j)];
                        }
                }
        }

    
    for(i = 0; i < n; i++)
        {
            for(j = i; j < n; j++)
                {
                    double s = 0;
                    for(k = j; k < n; k++)
                        {
                            s += A[IJ_EL(i, k)] * D[k] * A[IJ_EL(j, k)];
                        }
                    R[IJ_EL(i, j)] = s;
                    R[IJ_EL(j, i)] = s;
                }
        }
 
    return 0;
}
 
