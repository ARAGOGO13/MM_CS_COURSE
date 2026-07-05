#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "array_io.c"
#include "array_io.h"
#include "solve.h"

int main(int argc, char *argv[])
{
	int n, p;
	char *file_name = 0;
	double *a;
	double t;
	int res;
	if (!((argc == 4) && sscanf(argv[1], "%d", &n) == 1 && sscanf(argv[2], "%d", &p) == 1))
		{	
			printf("Usage: %s n p [file] \n", argv[0]);
			return 1;
		}

	if (p > n)
		{	
			printf("Error: %d numbers cannot be output from an array of length %d \n", p, n);
			return 2;
		}

	file_name = argv[3];

	a = (double*)malloc(n * sizeof(double));
	if (!a)
		{
			printf("Not enough memory! \n");
			return 3;
		}

	t = clock();
	res = task7(file_name, a, n);
	if (res < 0)
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
		        default:
		          printf("Unknown error\n");
		          break;
	      	}
	    free(a);
	    return 3;
		}
	t = (clock() - t) / CLOCKS_PER_SEC;
	printf("New array:\n");
	print_array(a, res, p);
	printf("%s : Task = %d Result = %d Elapsed = %.2f\n", argv[0], 7, res, t);
	free(a);
	return 0;
}