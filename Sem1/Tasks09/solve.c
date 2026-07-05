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

int diff_calculation(double *a, int n, int(*cmp)(double, double))
{
	int i, cnt = 0;
	for (i = 0; i < n - 1; i++)
		{	
			if ((*cmp)(a[i], a[i + 1]) > 0) cnt++;
		}
	return cnt;
}

int binary_search(double *a, int n, double x, int(*cmp)(double, double))
{
	int left = 0, right = n;
	int mid = 0;
	while (left != right)
		{	
			mid = (left + right) / 2;
			if ((*cmp)(a[mid], x) < 0)
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

void merge_arrays(double *a, double *b, double *c, int n, int m, int(*cmp)(double, double))
{
	int i = 0, j = 0, k = 0;
  while (i < n && j < m) 
  {
    if ((*cmp)(a[i], b[j]) < 0)
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

int dividing_array(double *a, int n, double x, int(*cmp)(double, double))
{
	int i = 0, j = n - 1;
	while (i <= j)
		{
			while (i < n)
				{
					if ((*cmp)(a[i], x) >= 0) break;
					i++;
				} 
			while (j >= 0)
				{
					if ((*cmp)(a[j], x) <= 0) break;
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

void bubble_sort(double *a, int n, int(*cmp)(double, double))
{
	int i = 0, j = 0;
	for (i = 0; i < n; i++) 
		{
		  for (j = 0; j < n - i - 1; j++) 
			  {
				  if ((*cmp)(a[j], a[j + 1]) > 0) 
					  {
						  swap(a + j, a + j + 1);
					  }
			  }
	  }
}

void find_min_sort(double *a, int n, int(*cmp)(double, double))
{
	int i, k, ind_min;
	double a_min;
	for (k = n - 1; k >= 1; k--)
		{
			a_min = a[0];
			ind_min = 0;
			for (i = 1; i <= k; i++)
				{
					if ((*cmp)(a_min, a[i]) < 0) 
						{	
							a_min = a[i];
							ind_min = i;
						}
				}
			if (ind_min != k) swap(a + k, a + ind_min);
		}
}

void linear_insert_sort(double *a, int n, int(*cmp)(double, double))
{
	int i, j;
	double ins_el;
  for (i = 1; i < n; i++) {
    ins_el = a[i];
    j = i - 1;
    while (j >= 0 && (*cmp)(a[j], ins_el) > 0) {
      a[j + 1] = a[j];
      j --;
    }
    a[j + 1] = ins_el;
  }
}

void binary_insert_sort(double *a, int n, int(*cmp)(double, double))
{
	int i, j, ind;
	double ins_el;
  for (i = 1; i < n; i++) {
  	j = i - 1;
    ins_el = a[i];
    ind = binary_search(a, i, ins_el, *cmp);

    while (j >= ind)
    	{
    		a[j + 1] = a[j];
    		j--;
    	}
    a[ind] = ins_el;
  }
}

void merge_sort(double *a, double *b, int n, int(*cmp)(double, double))
{
	double *c = a;
	double *a_original = a;
	int part_len, i;
	for (part_len = 1; part_len < n; part_len *= 2) 
	{
		int last_len = n % (2 * part_len);
	  for (i = 0; i <= n - 2 * part_len; i += 2 * part_len) 
	    {
	      merge_arrays(a + i, a + i + part_len, b + i, part_len, part_len, *cmp);
	    }
	  if (last_len > part_len) 
	    {
	      merge_arrays(a + i, a + i + part_len, b + i, part_len, n % part_len, *cmp);
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

void quick_sort(double *a, int n, int(*cmp)(double, double))
{
	int ind;
	if (n <= 1) return;
	while (n > 1)
		{
			ind = dividing_array(a, n, a[n / 2], *cmp);
			if (ind == n) ind = n - 1;
			if (ind == 0) ind = 1;
			if (ind < n - ind)
				{
					quick_sort(a, ind, *cmp);
					a = a + ind;
					n -= ind;
				}
			else 
				{
					quick_sort(a + ind, n - ind, *cmp);
					n = ind;
				}
		}
}

void tournament_sort(double *a, int n, int(*cmp)(double, double)) 
{
    int k, parent_index, left_index, right_index, index, largest, j;
    double mid;
    for (k = 1; k < n; k++) 
    {
      index = k;
      mid = a[k];
      while (index > 0) {
          parent_index = (index - 1) / 2;
          if ((*cmp)(mid, a[parent_index]) <= 0) break;
          a[index] = a[parent_index];
          index = parent_index;
      }
      a[index] = mid;
    }


    for (k = n - 1; k > 0; k--) {
        swap(a + k, a + 0);

        j = 0;
        while (1) 
        {
          left_index = 2 * j + 1;
          right_index = 2 * j + 2;

          if (left_index >= k) break;

          largest = j;
          if (right_index < k && (*cmp)(a[left_index], a[right_index]) < 0) 
          {
            largest = right_index;
          } 
          else if (left_index < k) 
          {
            largest = left_index;
          }
          if ((*cmp)(a[j], a[largest]) >= 0) break;

          swap(a + j, a + largest);
          j = largest;
        }
    }
}