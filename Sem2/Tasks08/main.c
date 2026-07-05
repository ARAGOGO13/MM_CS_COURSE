#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include "solve.h"

int main(int argc, char *argv[])
{
	double *a, *x, *d, *check_a;
	int n, p, k;
	char *filename = 0;
	double t = 0, r1 = 0, r2 = 0;
	int task = 6;
	int res = 0;

	if (!((argc == 4 || argc == 5)
			&& sscanf(argv[1], "%d", &n) == 1
			&& sscanf(argv[2], "%d", &p) == 1
			&& sscanf(argv[3], "%d", &k) == 1
                        && k >= 0 && k <= 4
                        && ((k == 0) == (argc == 5))))
		{
			printf("Usage: %s n p k [name] \n", argv[0]);
			return 0;
		}
	if (argc == 5) filename = argv[4];


	a = (double *) malloc (n * n * sizeof(double));
	if (!a)
		{
			printf("Not enough memory\n");
			return 2;
		}
	x = (double *) malloc (n * n * sizeof(double));
	if (!x)
		{
			free(a);
			printf("Not enough memory\n");
			return 2;
		}
	d = (double *) malloc (n * sizeof(double));
	if (!d)
		{
			free(a);
			free(x);
			printf("Not enough memory\n");
			return 2;
		}

	if (filename)
		{
			io_status ret;
			ret = read_matrix(a, n, n, filename);
			check_a = (double *) malloc (n * n * sizeof(double));
			if (!check_a)
				{
					free(a);
					free(x);
					free(d);
					printf("Not enough memory\n");
					return 2;
				}
			ret = read_matrix1(check_a, n, n, filename);
			do
				{
					switch (ret)
						{
							case SUCCESS:
								continue;
							case ERROR_OPEN:
								printf("Can not open %s\n", filename);
								break;
							case ERROR_READ:
								printf("Can not read from %s\n", filename);
								break;
							case ERROR_MEM:
								printf("Not enough memory");
								break;
						}
					free(a);
			        free(x);
			        free(d);
					free(check_a);
					return 3;
			
				} while (0);
			res = is_symmetric (check_a, n);
			if (!res)
				{
					free(a);
			        free(x);
			        free(d);
			        free(check_a);
					printf("Error: Matrix is not symmetric\n");
					return 4;
				}
			free(check_a);
			ret = read_matrix(a, n, n, filename);
			do
				{
					switch (ret)
						{
							case SUCCESS:
								continue;
							case ERROR_OPEN:
								printf("Can not open %s\n", filename);
								break;
							case ERROR_READ:
								printf("Can not read from %s\n", filename);
								break;
							case ERROR_MEM:
								printf("Not enough memory");
								break;
						}
					free(a);
					free(x);
					free(d);
					return 3;
			
				} while (0);
		}
	else
		{
			init_matrix(a, n, n, k);
		}
	printf("Initial matrix:\n");
	print_matrix(a, n, n, p);

	t = clock();
	res = cholesky_inverse(n, a, x, d);
	t = (clock() - t) / CLOCKS_PER_SEC;

	if (res == -1)
		{
			printf("Cannot solve Cholesky for n = %d s = %d file = %s\n", n, k, filename);
			free(a);
			free(x);
			free(d);
			return 3;
		}


	printf("Result matrix:\n");
	print_matrix(x, n, n, p);

	if (filename)
		{
			io_status ret;
			ret = read_matrix(a, n, n, filename);
			do
				{
					switch (ret)
						{
							case SUCCESS:
								continue;
							case ERROR_OPEN:
								printf("Can not open %s\n", filename);
								break;
							case ERROR_READ:
								printf("Can not read from %s\n", filename);
								break;
							case ERROR_MEM:
								printf("Not enough memory");
								break;
						}
					free(a);
					free(x);
					free(d);
					return 3;
			
				} while (0);
		}
	else
		{
			init_matrix(a, n, n, k);
		}

	r_solver (a, x, n, &r1, &r2);
	printf ("%s : Task = %d Res1 = %e Res2 = %e Elapsed = %.2f K = %d N = %d\n", argv[0], task, r1, r2, t, k, n);

	free(a);
	free(x);
	free(d);
	return 0;
}
