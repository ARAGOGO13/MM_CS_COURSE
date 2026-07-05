#include <stdio.h>
#include <math.h>
#include "solve.h"
#include "array_io.h"

int binary_search(double *a, int n, double value, int type)
{
    int low = 0, high = n - 1;
    while (low <= high)
    {
        int mid = (low + high) / 2;
        if (fabs(a[mid] - value) < EPS)
        {
            return mid;
        }
        else if ((a[mid] < value) * type + (a[mid] > value) * (1 - type))
        {
            low = mid + 1;
        }
        else
        {
            high = mid - 1;
        }
    }
    return low;
}

int filter_array1(double *a, int n)
{
	int i, j = 0, arithmetic_cnt = 0;
	double arithmetic_sum = 0, arithmetic_mean;
	double tmp = 0;
	for (i = 1; i < n - 1; i++)
		{	
			if (a[i - 1] * a[i + 1] >= 0)
				{
				  tmp = a[i] * a[i];
				  if (a[i] < 0) tmp *= -1;
				  if (tmp < (a[i + 1] * a[i - 1]))
						{	
							arithmetic_sum += a[i];
							arithmetic_cnt ++;
						}
				}
		}

	if (arithmetic_cnt != 0)
		{
			arithmetic_mean = arithmetic_sum / arithmetic_cnt;
			for (i = 0; i < n; i++)
			{
				if (a[i] >= arithmetic_mean) a[j++] = a[i];
			}
			return j;
		}
	
	return n;
}

int filter_array2(double *a, int n)
{
	int i, flag = 0, j = 0, arithmetic_cnt = 0;
	double arithmetic_sum = 0, arithmetic_mean;
	for (i = 0; i < n - 1; i++)
		{	
			if (fabs(a[i] - a[i + 1]) < EPS)
				{	
					arithmetic_sum += a[i];
					arithmetic_cnt ++;
					flag = 1;
				}
			else if (flag)
				{
					arithmetic_sum += a[i];
					arithmetic_cnt ++;
					flag = 0;
				}
		}

	if (flag)
		{
			arithmetic_sum += a[i];
			arithmetic_cnt ++;
		}

	if (arithmetic_cnt != 0)
	{
		arithmetic_mean = arithmetic_sum / arithmetic_cnt;
		for (i = 0; i < n; i++)
		{
			if (a[i] >= arithmetic_mean) a[j++] = a[i];
		}
		return j;
	}
	
	return n;
}

int filter_array3(double *a, int n)
{
	int i, mcurr_length = 0, flag = 0, j = 0, arithmetic_cnt = 0;
	double arithmetic_sum = 0, mcurr_sum = 0, arithmetic_mean;
	for (i = 0; i < n - 1; i++)
		{	
			if (a[i] <= a[i + 1])
				{	
					arithmetic_sum += a[i];
					arithmetic_cnt ++;
					flag = 1;
				}
			else if (flag)
				{
					if (arithmetic_cnt > mcurr_length)
						{	
							mcurr_sum = arithmetic_sum + a[i];
							mcurr_length = arithmetic_cnt + 1;
							arithmetic_mean = mcurr_sum / mcurr_length;
						}
					flag = 0;
					arithmetic_sum = 0;
					arithmetic_cnt = 0;
				}
		}
	if (arithmetic_cnt > mcurr_length)
		{	
			mcurr_sum = arithmetic_sum + a[i];
			mcurr_length = arithmetic_cnt + 1;
			arithmetic_mean = mcurr_sum / mcurr_length;
		}	

	for (i = 0; i < n; i++)
	{
		if (a[i] >= arithmetic_mean) a[j++] = a[i];
	}
	return j;
}

int filter_array4(int k, double *a, int n)
{
	int i, j = 0, flag = 0, const_cnt = 1;
	double mcurr_const_el;
	for (i = 0; i < n - 1; i++)
		{	
			if (fabs(a[i] - a[i + 1]) < EPS)
				{
					const_cnt ++;
					if (const_cnt >= k)
						{	
							if (flag == 0)
								{
									mcurr_const_el = a[i];
								}
							if (a[i] > mcurr_const_el)
								{	
									mcurr_const_el = a[i];
								}
							flag = 1;
						}
				}
			else
				{	
					const_cnt = 1;
				}
		}
	if (flag)
		{
			for (i = 0; i < n; i++)
				{
					if (a[i] >= mcurr_const_el) a[j++] = a[i];
				}
			return j;
		}
	return n;
}

int filter_array5(int k, double *a, int n)
{
	int i, j = 0, cnt = 0, arithmetic_cnt = 0;
	double arithmetic_sum = 0, summ = 0, arithmetic_mean; 
	for (i = 0; i < n - 1; i++)
		{	
			if (a[i] >= a[i + 1])
				{
					cnt ++;
					summ += a[i];
					while (i != n - 1 && a[i] >= a[i + 1])
						{	
							cnt ++;
							summ += a[i + 1];
							i++;
						}
					if (cnt >= k)
						{
							arithmetic_cnt += cnt;
							arithmetic_sum += summ;
						}
					summ = 0; 
					cnt = 0;
				}
		}
	if (arithmetic_cnt != 0)
		{
			arithmetic_mean = arithmetic_sum / arithmetic_cnt;
			for (i = 0; i < n; i++)
				{
					if (a[i] >= arithmetic_mean) a[j++] = a[i];
				}
			return j;
		}
	return n;
}

int task6(char *file, double *a, int n)
{
    FILE *fp;
    int i = 0, j, k, ind;
    double curr, prev;
    int cnt = 0;
    int type_of_bin = 1;

    if (!(fp = fopen(file, "r"))) return ERROR_OPEN;

    if (fscanf(fp, "%lf", &prev) != 1)
    {
        if (feof(fp))
        {
            fclose(fp);
            return 0;
        }
        fclose(fp);
        return ERROR_READ;
    }
    
    while (fscanf(fp, "%lf", &curr) == 1)
    {
        if (fabs(prev - curr) < EPS)
        {
            for (j = 0; j < i; j++)
            {
                if (fabs(a[j] - curr) > EPS)
                {
                    cnt++;
                }
            }
            if (cnt == i)
            {
                ind = binary_search(a, i, curr, type_of_bin);
                if (ind != -1)
                {
                    i++;
                    if (i == n + 1) i--;
                    for (k = i - 1; k >= ind; k--)
                    {
                        a[k + 1] = a[k];
                    }
                    a[ind] = curr;
                }
                else
                {
                    a[i] = curr;
                    i++;
                    if (i == n + 1) i--;
                }
            }
            cnt = 0;
        }
        prev = curr;
    }

    if (!feof(fp))
    {
        fclose(fp);
        return ERROR_READ;
    }

    fclose(fp);
    return i;
}

int task7(char *file, double *a, int n)
{
    FILE *fp;
    int i = 0, j, k, ind;
    double curr, prev2, prev1;
    int type_of_bin = 0;
    int cnt = 0;

    if (!(fp = fopen(file, "r"))) return ERROR_OPEN;

    if (fscanf(fp, "%lf", &prev2) != 1)
    {
        if (feof(fp))
        {
            fclose(fp);
            return 0;
        }
        fclose(fp);
        return ERROR_READ;
    }

   	if (fscanf(fp, "%lf", &prev1) != 1)
    {
        if (feof(fp))
        {
            fclose(fp);
            return 0;
        }
        fclose(fp);
        return ERROR_READ;
    }
    
    while (fscanf(fp, "%lf", &curr) == 1)
    {
        if (prev2 < prev1 && prev1 > curr)
        {
          for (j = 0; j < i; j++)
            {
                if (fabs(a[j] - prev1) > EPS)
                	{
                    cnt++;
                	}
            }
          if (cnt == i)
            {
              ind = binary_search(a, i, prev1, type_of_bin);
              if (ind != -1)
	              {
	                  i++;
	                  if (i == n + 1) i--;
	                  for (k = i - 1; k >= ind; k--)
	                  {
	                      a[k + 1] = a[k];
	                  }
	                  a[ind] = prev1;
	              }
              else
	              {
	                  a[i] = prev1;
	                  i++;
	                  if (i == n + 1) i--;
	              }
            }
            cnt = 0;
        }
        prev2 = prev1;
        prev1 = curr;
    }

    if (!feof(fp))
    {
        fclose(fp);
        return ERROR_READ;
    }

    fclose(fp);
    return i;
}