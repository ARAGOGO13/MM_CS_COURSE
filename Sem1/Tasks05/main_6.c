#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "array_io.c"
#include "array_io.h"
#include "solve.h"

int main(int argc, char *argv[])
{
	int n, p, s;
	double x;
	char *file_name = 0;
	double *a;
	double t;
	int res;
	if (!((argc == 5 || argc == 6) && sscanf(argv[1], "%lf", &x) == 1 && sscanf(argv[2], "%d", &n) == 1 && sscanf(argv[3], "%d", &p) == 1 && sscanf(argv[4], "%d", &s) == 1))
		{	
			printf("Usage: %s x n p s [file] \n", argv[0]);
			return 1;
		}

	if (p > n)
		{	
			printf("Error: %d numbers cannot be output from an array of length %d \n", p, n);
			return 2;
		}

	if ((s == 0 && argc != 6) || (s != 0 && argc == 6))
		{
			printf("Need to set s = 0 or add file_name \n");
			return 4;
		}


	if (argc == 6) file_name = argv[5];
	a = (double*)malloc(n * sizeof(double));
	if (!a)
		{
			printf("Not enough memory! \n");
			return 3;
		}

	if (file_name)
		{	
			res = read_array(a, n, file_name);
			if (res != SUCCESS)
				{	
					switch (res) 
			      {
			        case ERROR_OPEN:
			          printf("Error: Can not read from %s \n", file_name);
			          break;
			        case ERROR_READ:
			          printf("Error: reading the file %s \n", file_name);
			          break;
			        case EMPTY_FILE:
			        	printf("File %s is empty \n", file_name);
			        	break;
			        case NOT_ENOUGH_ELEMENTS:
			        	printf("Not enough elements in file %s \n", file_name);
			        	break;
			        default:
			          printf("Unknown error\n");
			           break;
			      }
			    free(a);
			    return 3;
				}
		}
		else init_array(a, n, s);
		print_array(a, n, p);
		t = clock();
		res = delete_small_elements(x, a, n);
		t = (clock() - t) / CLOCKS_PER_SEC;
		printf("New array:\n");
		print_array(a, res, p);
		printf("%s : Task = %d Result = %d Elapsed = %.2f\n", argv[0], 6, res, t);
		free(a);
		return 0;
}