#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include "solve.h"
#include "array_io.h"

void delete_column (double *a, int m, int n, int column)
{
	int i = 0, j = 0;
	for (i = 0; i < m; i ++) 
		{
		    for (j = 0; j < n - 1; j ++) 
			    {
			    	int original_col = (j < column) ? j : j + 1;
			    	a[i * (n - 1) + j] = a[i * n + original_col];
			    }
	  	}
}

void delete_row_column (double *a, int m, int n, int row, int column)
{
	int i = 0, j = 0;
	for (i = 0; i < m - 1; i ++) 
		{
	        for (j = 0; j < n - 1; j ++) 
		        {
		            int original_row = (i < row) ? i : i + 1;
		            int original_col = (j < column) ? j : j + 1;
		            a[i * (n - 1) + j] = a[original_row * n + original_col];
		        }
	    }
}

int delete_min_el_column (double *a, int m, int n)
{	
	int i = 0, j = 0, min_column = 0;
	double min_el = fabs(a[0]);
	for (i = 0; i < m; i ++)
		{
			for (j = 0; j < n; j ++)
				{
					double el = fabs(a[i * n + j]);
					if (el < min_el)
						{
							min_el = el;
							min_column = j;
						}
				}
		}

	delete_column (a, m, n, min_column);

	return min_column + 1;
}

int delete_min_el_row_column (double *a, int m, int n)
{	
	int i = 0, j = 0, min_column = 0, min_row = 0;
	double min_el = fabs(a[0]);
	for (i = 0; i < m; i ++)
		{
			for (j = 0; j < n; j ++)
				{
					double el = fabs(a[i * n + j]);
					if (el < min_el)
						{
							min_el = el;
							min_column = j;
							min_row = i;
						}
				}
		}

	delete_row_column (a, m, n, min_row, min_column);

	return min_row * n + min_column;
}


int delete_min_sum_column (double *a, int m, int n)
{
	int i = 0, j = 0, min_column = 0;
	double min_sum = 0, column_sum = 0;

	for (i = 0; i < m; i ++)
		{
			min_sum += fabs(a[i * n]);
		}


	for (j = 1; j < n; j ++)
		{
			column_sum = 0;
			for (i = 0; i < m; i ++)
				{
					column_sum += fabs(a[i * n + j]);
				}
			if (column_sum < min_sum) 
				{
					min_sum = column_sum;
					min_column = j;
				} 
		}

	delete_column (a, m, n, min_column);

	return min_column + 1;
}

int delete_min_sum_row_column (double *a, int m, int n)
{
	int i = 0, j = 0, min_row = 0, min_column = 0;
	double row_sum = 0, column_sum = 0;
	double min_row_sum = 0, min_column_sum = 0;

	for (i = 0; i < m; i ++)
		{
			min_column_sum += fabs(a[i * n]);
		}

	for (j = 0; j < n; j ++)
		{
			min_row_sum += fabs(a[j]);
		}

	for (i = 1; i < m; i ++)
		{
			row_sum = 0;
			for (j = 0; j < n; j ++)
				{
					row_sum += fabs(a[i * n + j]);
				}
			if (row_sum < min_row_sum)
				{
					min_row = i;
					min_row_sum = row_sum;
				}
		}

	for (j = 1; j < n; j ++)
		{
			column_sum = 0;
			for (i = 0; i < m; i ++)
				{
					column_sum += fabs(a[i * n + j]);
				}
			if (column_sum < min_column_sum) 
				{
					min_column = j;
					min_column_sum = column_sum;
				} 
		}

	delete_row_column (a, m, n, min_row, min_column);
	return min_row * n + min_column;
}

int delete_max_sum_column (double *a, int m, int n)
{
	int i = 0, j = 0, mx_column = 0;
	double mx_sum = 0, column_sum = 0;

	for (j = 0; j < n; j ++)
		{
			column_sum = 0;
			for (i = 0; i < m; i ++)
				{
					column_sum += fabs(a[i * n + j]);
				}
			if (j < m) column_sum -= a[j * n + j];
			if (column_sum > mx_sum) 
				{
					mx_sum = column_sum;
					mx_column = j;
				} 
		}

	delete_column(a, m, n, mx_column);
	return mx_column + 1;
}


int delete_max_sum_column_2 (double *a, int m, int n)
{
	int i = 0, j = 0, mx_column = 0, mn = 0;
	double mx_sum = 0, column_sum = 0;

	mn = m < n ? m : n;

	for (j = 0; j < mn; j ++)
		{
			column_sum = 0;
			for (i = 0; i < m; i ++)
				{
					column_sum += fabs(a[i * n + j] - a[j * n + j]);
				}
			if (column_sum > mx_sum) 
				{
					mx_sum = column_sum;
					mx_column = j;
				} 
		}

	delete_column(a, m, n, mx_column);
	return mx_column + 1;
}

int delete_min_variance_column (double *a, int m, int n)
{
	int i = 0, j = 0, min_column = 0;
	double column_sum = 0, column_sum_sq = 0, column_average = 0, variance = 0, min_variance = 0, tmp = 0;

	for (i = 0; i < m; i ++)
		{
			tmp = a[i * n + j];
			column_sum += tmp;
			column_sum_sq += tmp * tmp;
		}

	column_average = column_sum / m;
	min_variance = (column_sum_sq / m) - (column_average * column_average);


	for (j = 1; j < n; j ++)
		{
			column_sum = 0;
			column_sum_sq = 0;

			for (i = 0; i < m; i ++)
				{
					tmp = a[i * n + j];
					column_sum += tmp;
					column_sum_sq += tmp * tmp;
				}

			column_average = column_sum / m;
			variance = (column_sum_sq / m) - (column_average * column_average);

			if (variance < min_variance)
				{
					min_variance = variance;
					min_column = j;
				}
		}

	delete_column(a, m, n, min_column);
	return min_column + 1;
}

int delete_max_sum_row_column_2 (double *a, int m, int n)
{
	int i = 0, j = 0, k = 0, max_row = 0, max_column = 0;
	double row_sum = 0, column_sum = 0, sum = 0;
	double max_sum = 0;

	for (i = 0; i < m; i ++)
		{
			row_sum = 0;
			for (j = 0; j < n; j ++) 
				{
            		row_sum += fabs(a[i * n + j]);
        		}
			for (j = 0; j < n; j ++)
				{
					column_sum = 0;
					for (k = 0; k < m; k ++)
						{
							column_sum += fabs(a[k * n + j]);
						}
					sum = column_sum + row_sum - 2 * a[i * n + j];
					if (sum > max_sum)
						{
							max_row = i;
							max_column = j;
							max_sum = sum;
						}
				}

		}

	delete_row_column (a, m, n, max_row, max_column);
	return max_row * n + max_column;
}

int delete_max_sum_row_column_3 (double *a, int m, int n)
{
	int i = 0, j = 0, k = 0, max_row = 0, max_column = 0;
	double sum = 0;
	double max_sum = 0, tmp = 0;

	for (i = 0; i < m; i ++)
		{
			for (j = 0; j < n; j ++)
				{
					sum = 0;
					tmp = a[i * n + j];
					for (k = 0; k < n; k ++)
						{
							sum += fabs(a[i * n + k] - tmp);
						}
					for (k = 0; k < m; k ++)
						{
							sum += fabs(a[k * n + j] - tmp);
						}
					if (sum > max_sum)
						{
							max_row = i;
							max_column = j;
							max_sum = sum;
						}
				}

		}

	delete_row_column (a, m, n, max_row, max_column);
	return max_row * n + max_column;
}

int delete_min_variance_row_column (double *a, int m, int n)
{
	int i = 0, j = 0, k = 0, min_row = 0, min_column = 0;
	double row_sum = 0, column_sum = 0, row_variance = 0, column_variance = 0, tmp = 0;
	double row_sum_sq = 0, column_sum_sq = 0, min_row_variance = 0, min_column_variance = 0;

	for (k = 0; k < n; k ++)
		{
			tmp = a[k];
			row_sum += tmp;
			row_sum_sq += tmp * tmp;
		}
	min_row_variance = (row_sum_sq / n) + (row_sum * row_sum) / (n * n);

	for (k = 0; k < m; k ++)
		{
			tmp = a[k * n];
			column_sum += tmp;
			column_sum_sq += tmp * tmp;
		}
	min_column_variance = (column_sum_sq / m) - (column_sum * column_sum) / (m * m);

	for (i = 0; i < m; i ++)
		{
			row_sum = 0, row_sum_sq = 0;
			for (j = 0; j < n; j ++)
				{
					tmp = a[i * n + j];
					row_sum += tmp;
					row_sum_sq += tmp * tmp;
				}
			row_variance = (row_sum_sq / n) - (row_sum * row_sum) / (n * n);
			if (row_variance < min_row_variance)
				{
					min_row = i;
					min_row_variance = row_variance;
				}
		}
	for (j = 0; j < n; j ++)
		{
			column_sum = 0, column_sum_sq = 0;
			for (i = 0; i < m; i ++)
				{
					tmp = a[i * n + j];
					column_sum += tmp;
					column_sum_sq += tmp * tmp;
				}
			column_variance = (column_sum_sq / m) - (column_sum * column_sum) / (m * m);
			if (column_variance < min_column_variance)
				{
					min_column = j;
					min_column_variance = column_variance;
				}
		}

	delete_row_column (a, m, n, min_row, min_column);
	return min_row * n + min_column;
}



