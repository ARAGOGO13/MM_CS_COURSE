#include <stdio.h>
#include <math.h>
#include "solve.h"
#include "array_io.h"

int is_polyndrom(double *a, int n)
{
	int i;
	for (i = 0; i < n; i++)
		{	
			if (fabs(a[i] - a[n - i - 1]) > EPS)
				{
					return 0;
				}
		}
	return 1;
}

int reverse_array(double *a, int n)
{
	int i;
	double tmp;
	for (i = 0; i < n / 2; i++) 
		{	
			tmp = a[n - i - 1];
			a[n - i - 1] = a[i];
			a[i] = tmp; 
		}
	return 0;
}

int replace_with_means(double *a, int n)
{
	int i;
	double prev2, prev1 = a[0];
	for (i = 1; i < n - 1; i++) 
		{	
			prev2 = a[i];
			a[i] = (prev1 + a[i + 1]) / 2.;
			prev1 = prev2;
		}
	return 0;
}

int right_shift(double *a, int n)
{
	int i;
	double tmp = a[n - 1];
	for (i = n - 1; i > 0; i--) 
		{	
			a[i] = a[i - 1];
		}
	a[0] = tmp;
	return 0;
}

int right_shift_k(int k, double *a, int n)
{
	int i, new_index, j = 0;
	double tmp1, tmp2;
	while (k < 0)
		{	
			k += n;
		}
	k %= n;

	tmp1 = a[0];
	for (i = 0; i < n; i++) 
		{
			new_index = ((i + 1) * k) % n + j;
			tmp2 = a[new_index];
			a[new_index] = tmp1;
			tmp1 = tmp2;
			if (i != n - 1 && new_index == 0)
				{
					j ++;
					tmp1 = a[((i + 1) * k) % n + j];
				}
		}
	return 0;
}

int delete_small_elements(double x, double *a, int n)
{
	int i, j = 0;
  for (i = 0; i < n; i++) 
    {
      if (a[i] >= x) 
      	{
          a[j++] = a[i];
        }
    }
  return j;
}


int num_of_identical(const char *file_b, double *a, int n)
{
	FILE *fp; int i = 0;
	double x;
	int cnt = 0;
	if (!(fp = fopen(file_b, "r"))) return ERROR_OPEN;

	while (fscanf(fp, "%lf", &x) == 1)
		{
			for (i = 0; i < n; i++)
				{	
					if (fabs(x - a[i]) < EPS) 
						{
							cnt++;
							break;
						}
				}
		}
	return cnt;
}

int num_of_occurrences(const char *file_b, double *a, int n)
{
	FILE *fp;
	double x;
	int oc_cnt = 0;
	int cnt = 0;

	if (!(fp = fopen(file_b, "r"))) return ERROR_OPEN;

	while (fscanf(fp, "%lf", &x) == 1)
		{
			if (fabs(x - a[cnt]) < EPS) cnt++;
			else
				{
					if (fabs(x - a[0]) < EPS) cnt = 1;
					else cnt = 0;
				}
			if (cnt == n)
				{	
					oc_cnt ++;
					cnt = 0;
				}
		}
	fclose(fp);
	return oc_cnt;
}

int connect_arrays(const double *a, const double *b, double *c, int n, int m)
{
	int i = 0, j = 0, k = 0;
  while (i < n && j < m) 
  {
    if (a[i] < b[j])
    	{
    		c[k] = a[i];
    		k++;
    		i++;
    	}
    else if (a[i] > b[j])
    	{	
    		c[k] = b[j];
    		k++;
    		j++;
    	}
    else
    	{	
    		c[k] = a[i];
    		i++;
    		j++;
    		k++;
    	}
  }

  for (; i < n; i++)
  	{	
  		c[k] = a[i];
  		k++;
  	}
  for (; j < m; j++)
  	{	
  		c[k] = b[j];
  		k++;
  	}

  return k;
}

int delete_duplicates(double *a, double *b, int n, int m)
{
	int i = 0, j = 0, l = 0;
	int flag = 0;

	while (i < n)
		{
			flag = 0;
			for (j = 0; j < m; j++)
				{	
					if (fabs(a[i] - b[j]) < EPS)
					{
						flag = 1;
						break;
					} 
				}
			if (!flag) a[l++] = a[i];
			i++;
		}
	return l;
}