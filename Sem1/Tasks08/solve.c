#include <stdio.h>
#include <math.h>
#include "solve.h"
#include "array_io.h"

void swap(double *a, double *b)
{
	double c = *a;
	*a = *b;
	*b = c;
}

int diff_calculation(double *a, int n)
{
	int i, cnt = 0;
	for (i = 0; i < n - 1; i++)
		{	
			if (a[i] > a[i + 1]) cnt++;
		}
	return cnt;
}

int binary_search(double *a, int n, double x)
{
	int left = 0, right = n;
	int mid = 0;
	while (left != right)
		{	
			mid = (left + right) / 2;
			if (a[mid] < x)
				{	
					left = mid + 1;
				}
			else
				{	
					right = mid;
				}
		}
	return left;
}

void merge_arrays(double *a, double *b, double *c, int n, int m)
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
    else
    	{	
    		c[k] = b[j];
    		k++;
    		j++;
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
}

int dividing_array(double *a, int n, double x)
{
	int i = 0, j = n - 1;
	while (i <= j)
		{
			while (i < n)
				{
					if (a[i] >= x) break;
					i++;
				} 
			while (j >= 0)
				{
					if (a[j] <= x) break;
					j--;
				}
			if (i <= j) 
				{
					if (i == j) return i;
					swap(a + i, a + j);
					i++;
					j--;
				}
		}
	return i;
}

void bubble_sort(double *a, int n)
{
	int i = 0, j = 0;
	for (i = 0; i < n; i++) 
		{
		  for (j = 0; j < n - i - 1; j++) 
			  {
				  if (a[j] > a[j + 1]) 
					  {
						  swap(a + j, a + j + 1);
					  }
			  }
	  }
}

void find_min_sort(double *a, int n)
{
	int i, k, ind_min;
	double a_min;
	for (k = n - 1; k > 1; k--)
		{
			a_min = a[0];
			ind_min = 0;
			for (i = 1; i <= k; i++)
				{
					if (a_min < a[i]) 
						{	
							a_min = a[i];
							ind_min = i;
						}
				}
			if (ind_min != k) swap(a + k, a + ind_min);
		}
}

void linear_insert_sort(double *a, int n)
{
	int i, j;
	double ins_el;
  for (i = 1; i < n; i++) {
    ins_el = a[i];
    j = i - 1;
    while (j >= 0 && a[j] > ins_el) {
      a[j + 1] = a[j];
      j --;
    }
    a[j + 1] = ins_el;
  }
}

void binary_insert_sort(double *a, int n)
{
	int i, j, ind;
	double ins_el;
  for (i = 1; i < n; i++) {
  	j = i - 1;
    ins_el = a[i];
    ind = binary_search(a, i, ins_el);

    while (j >= ind)
    	{
    		a[j + 1] = a[j];
    		j--;
    	}
    a[ind] = ins_el;
  }
}

void merge_sort(double *a, double *b, int n)
{
	double *c = a;
	double *a_original = a;
	int part_len, i;
	for (part_len = 1; part_len < n; part_len *= 2) 
	{
		int last_len = n % (2 * part_len);
	  for (i = 0; i <= n - 2 * part_len; i += 2 * part_len) 
	    {
	      merge_arrays(a + i, a + i + part_len, b + i, part_len, part_len);
	    }
	  if (last_len > part_len) 
	    {
	      merge_arrays(a + i, a + i + part_len, b + i, part_len, n % part_len);
	  	}
		else if (last_len > 0)
			{
				for (; i < n; i++)
					{
						b[i] = a[i];
					}
			}
		c = a; 
		a = b;
		b = c;
	}
	if (a != a_original)
		{
			for (i = 0; i < n; i++)
				{
					a_original[i] = a[i];
				}
		}
}

void quick_sort(double *a, int n)
{
	int ind;
	if (n <= 1) return;
	while (n > 1)
		{
			ind = dividing_array(a, n, a[n / 2]);
			if (ind == n) ind = n - 1;
			if (ind == 0) ind = 1;
			if (ind < n - ind)
				{
					quick_sort(a, ind);
					a = a + ind;
					n -= ind;
				}
			else 
				{
					quick_sort(a + ind, n - ind);
					n = ind;
				}
		}
}

void tournament_sort(double *a, int n)
{
	int j, ind;
	int i, i1, i2;
	for (ind = 1; ind < n; ind++)
		{
			while (ind > 0)
				{
					j = (ind - 1) / 2;
					if (a[ind] > a[j]) swap(a + ind, a + j);
					else break;
					ind = j;
				}
		}
	for (ind = ind - 1; ind > 0; ind--)
		{
			swap(a, a + ind);
			for (i = 0; i < ind;)
				{
					i1 = 2 * i + 1;
					i2 = 2 * i + 2;
					if (i1 >= ind) break;
					if (i2 < ind && a[i1] < a[i2]) i1 = i2;
					if (a[i] < a[i1])
						{
							swap(a + i, a + i1);
							i = i1;
						}
					else break;
				}
		}
}