#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include "array_io.c"
#include "array_io.h"
#include "solve.h"

int main(int argc, char *argv[])
{
	int n, m, pa, pb, sa, sb, diff;
	char *file_name_b = 0;
	char *file_name_a = 0;
	double *a, *b, *c;
	double t;
	int res;
	if (!((7 <= argc && argc <= 9) && sscanf(argv[1], "%d", &n) == 1 && 
		sscanf(argv[2], "%d", &pa) == 1 && sscanf(argv[3], "%d", &sa) == 1))
		{	
			printf("Usage: %s n p s [file] \n", argv[0]);
			return 1;
		}

	if (pa > n)
		{	
			printf("Error: %d numbers cannot be output from an array of length %d \n", pa, n);
			return 2;
		}

	if (sscanf(argv[4], "%d", &m) != 1)
		{
			file_name_a = argv[4];
			if (!(sscanf(argv[5], "%d", &m) == 1 && 
				sscanf(argv[6], "%d", &pb) == 1 && 
				sscanf(argv[7], "%d", &sb) == 1))
				{
					printf("Usage: %s n p s [file] \n", argv[0]);
					return 1;
				}
			if (argc == 9) file_name_b = argv[8];
		}

	else 
		{
			if (!(sscanf(argv[5], "%d", &pb) == 1 && 
				sscanf(argv[6], "%d", &sb) == 1))
				{
					printf("Usage: %s n p s [file] \n", argv[0]);
					return 1;
				}
			if (argc == 8) file_name_b = argv[7];
		}
	if (pb > m)
		{	
			printf("Error: %d numbers cannot be output from an array of length %d \n", pb, m);
			return 2;
		}

	a = (double*)malloc(n * sizeof(double));
	b = (double*)malloc(m * sizeof(double));
	if (!(a && b))
		{
			printf("Not enough memory! \n");
			return 3;
		}

	if (file_name_a)
		{	
			res = read_array(a, n, file_name_a);
			if (res != SUCCESS)
				{	
					switch (res) 
			      {
			        case ERROR_OPEN:
			          printf("Error: Can not read from %s \n", file_name_a);
			          break;
			        case ERROR_READ:
			          printf("Error: Invalid character in the file %s \n", file_name_a);
			          break;
			        case EMPTY_FILE:
			        	printf("File %s is empty \n", file_name_a);
			        	break;
			        case NOT_ENOUGH_ELEMENTS:
			        	printf("Not enough elements in file %s \n", file_name_a);
			        	break;
			        default:
			          printf("Unknown error\n");
			           break;
			      }
			    free(a);
			    return 3;
				}
		}
	else init_array(a, n, sa);
	printf("Array A:\n");
	print_array(a, n, pa);

	if (file_name_b)
		{	
			res = read_array(b, m, file_name_b);
			if (res != SUCCESS)
				{	
					switch (res) 
			      {
			        case ERROR_OPEN:
			          printf("Error: Can not read from %s \n", file_name_b);
			          break;
			        case ERROR_READ:
			          printf("Error: reading the file %s \n", file_name_b);
			          break;
			        case EMPTY_FILE:
			        	printf("File %s is empty \n", file_name_b);
			        	break;
			        case NOT_ENOUGH_ELEMENTS:
			        	printf("Not enough elements in file %s \n", file_name_b);
			        	break;
			        default:
			          printf("Unknown error\n");
			          break;
			      }
			    free(a);
			    free(b);
			    return 3;
				}
		}
	else init_array(b, m, sb);

	c = (double*)malloc((m + n) * sizeof(double));
	if (!c) printf("Not rnough memory\n");

	printf("Array B:\n");
	print_array(b, m, pb);

	t = clock();
	merge_arrays(a, b, c, n, m);
	t = (clock() - t) / CLOCKS_PER_SEC;

	printf("New array: \n");
	print_array(c, n + m, pa + pb);
	diff = diff_calculation(c, n + m);

	printf("%s : Task = %d Diff = %d Elapsed = %.2f\n", argv[0], 2, diff, t);
	free(a);
	free(b);
	free(c);
	return 0;
}