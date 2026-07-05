#include <stdio.h>
#include "array_io.h"

double f(int s, int n, int i)
{
	switch (s)
	{
		case 1:
			return i;
			break;
		case 2:
			return n - i;
			break;
		case 3:
			return (i / 2.0);
			break;
		case 4:
			return (n - i / 2.0);
			break;
		case 5:
			return (2.0 * i);
			break;
		case 6:
			return (n - 2.0 * i);
			break;
		default:
			return 1e308;
			break;
	}
}

int read_array(double *a, int n, const char *name)
{
	FILE *fp; int i;
	if (!(fp = fopen(name, "r"))) return ERROR_OPEN;
	
	for (i = 0; i < n; i++)
		{	
			if (fscanf(fp, "%lf", a + i) != 1)
				{	
					if(feof(fp))
						{	
							fclose(fp);
							return NOT_ENOUGH_ELEMENTS;
						}
					fclose(fp);
					return ERROR_READ;
				}
		}
	fclose(fp);
	return SUCCESS;
}

int init_array(double *a, int n, int s)
{
	int i;
	for (i = 0; i < n; i++)
		{	
			a[i] = f(s, n, i + 1);
		}
	return SUCCESS;
}

void print_array(const double *a, int n, int p)
{
	int i;
	for (i = 0; i < p && i < n; i++)
		{	
			printf("%lf\n", a[i]);
		}
}