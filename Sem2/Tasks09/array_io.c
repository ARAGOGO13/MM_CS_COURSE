#include <stdio.h>
#include "array_io.h"

io_status read_dots (double *x, double *y, int n, const char *name)
{
	int i = 0;
	FILE *fp;

	if (!(fp = fopen(name, "r")))
		{
			return ERROR_OPEN;
		}

	for (i = 0; i < n; i++) 
		{
	        if (fscanf(fp, "%lf", &x[i]) != 1) 
		        {
		            fclose(fp);
		            return ERROR_READ;
		        }
	        if (fscanf(fp, "%lf", &y[i]) != 1) 
		        {
		            fclose(fp);
		            return ERROR_READ;
		        }
	    }
	fclose(fp);
	return SUCCESS;
}

io_status read_dots_d (double *x, double *y, double *d, int n, const char *name)
{
	int i = 0;
	FILE *fp;

	if (!(fp = fopen(name, "r")))
		{
			return ERROR_OPEN;
		}

	for (i = 0; i < n; i++) 
		{
	        if (fscanf(fp, "%lf", &x[i]) != 1) 
		        {
		            fclose(fp);
		            return ERROR_READ;
		        }
	        if (fscanf(fp, "%lf", &y[i]) != 1) 
		        {
		            fclose(fp);
		            return ERROR_READ;
		        }
		    if (fscanf(fp, "%lf", &d[i]) != 1) 
		        {
		            fclose(fp);
		            return ERROR_READ;
		        }
	    }
	fclose(fp);
	return SUCCESS;
}

void print_array (double *a, int n)
{
	int i = 0;
	for (i = 0; i < n; i++) printf("%lf \n", a[i]);
}


