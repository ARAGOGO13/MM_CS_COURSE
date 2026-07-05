#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include "solve.h"

int main(int argc, char *argv[])
{
	double *a = 0, *x0 = 0, *x = 0, *b = 0;
	int n, m, p, ka, kx0;
	char *filename_a = 0, *filename_x0 = 0;
	double time = 0, t;
	double r1 = 0, r2 = 0;
	task_status res;
	int task = 2;

	if (!((7 <= argc && argc <= 9)
			&& sscanf(argv[1], "%lf", &t) == 1
			&& sscanf(argv[2], "%d", &m) == 1
			&& sscanf(argv[3], "%d", &n) == 1
			&& sscanf(argv[4], "%d", &p) == 1
			&& sscanf(argv[5], "%d", &ka) == 1
			&& ka >= 0 && ka <= 4))
		{
            printf("Usage: %s t m n p ka fa kx0 fx0 \n", argv[0]);
			return 0;
		}
	if (ka == 0)
		{
			if (argc < 8)
	    		{
	    			printf("Usage: %s t m n p ka fa kx0 fx0 \n", argv[0]);
	                return 0;
	    		}
			filename_a = argv[6];
			if (sscanf(argv[7], "%d", &kx0) != 1) 
				{
	                printf("Usage: %s t m n p ka fa kx0 fx0 \n", argv[0]);
					return 0;
				}
			if (kx0 == 0) filename_x0 = argv[8];
	        if (kx0 != 0 && argc == 9)
	            {
	                printf("Usage: %s t m n p ka fa kx0 fx0 \n", argv[0]);
	                return 0;
	            }
		}
	else
		{
			if (argc < 7)
				{
					printf("Usage: %s t m n p ka fa kx0 fx0 \n", argv[0]);
	                return 0;
				}
			if (sscanf(argv[6], "%d", &kx0) != 1) 
				{
                    printf("Usage: %s t m n p ka fa kx0 fx0 \n", argv[0]);
					return 0;
				}
            if (kx0 == 0) 
            	{
            		if (argc < 8)
                		{
                			printf("Usage: %s t m n p ka fa kx0 fx0 \n", argv[0]);
                            return 0;
                		}
		            filename_x0 = argv[7];
            	}
            if (kx0 != 0 && argc == 8)
                {
                    printf("Usage: %s t m n p ka fa kx0 fx0 \n", argv[0]);
                    return 0;
                }

		}

	a = (double *) malloc (n * n * sizeof(double));
	if (!a)
		{
			printf("Not enough memory\n");
			return 2;
		}
	if (filename_a)
		{
			io_status ret;
			ret = read_matrix(a, n, n, filename_a);
			do
				{
					switch (ret)
						{
							case SUCCESS:
								continue;
							case ERROR_OPEN:
								printf("Can not open %s\n", filename_a);
								break;
							case ERROR_READ:
								printf("Can not read from %s\n", filename_a);
								break;
							case ERROR_MEM:
								printf("Not enough memory");
								break;
						}
					free(a);
					return 3;
			
				} while (0);
		}
	else
		{
			init_matrix(a, n, n, ka);
		}

	x0 = (double *) malloc (n * 1 * sizeof(double));
	if (!x0)
		{
			free(a);
			printf("Not enough memory\n");
			return 2;
		}
	if (filename_x0)
		{
			io_status ret;
			ret = read_matrix(x0, n, 1, filename_x0);
			do
				{
					switch (ret)
						{
							case SUCCESS:
								continue;
							case ERROR_OPEN:
								printf("Can not open %s\n", filename_x0);
								break;
							case ERROR_READ:
								printf("Can not read from %s\n", filename_x0);
								break;
							case ERROR_MEM:
								printf("Not enough memory");
								break;
						}
					free(a);
                    free(x0);
					return 3;
			
				} while (0);
		}
	else
		{
			init_matrix(x0, n, 1, kx0);
		}

	x = (double *) malloc (n * sizeof(double));
	if (!x)
		{
			free(a);
			free(x0);
			printf("Not enough memory\n");
			return 2;
		}

	b = (double *) malloc (n * sizeof(double));
	if (!b)
		{
			free(a);
			free(x0);
			free(x);
			printf("Not enough memory\n");
			return 2;
		}

	compute_vector_b (a, b, n);


	printf("Initial matrix A:\n");
	print_matrix(a, n, n, p);

	printf("Initial vector X0:\n");
	print_matrix(x0, 1, n, p);

	printf("Initial vector B:\n");
	print_matrix(b, 1, n, p);

	time = clock();
	res = task02 (a, x0, x, b, n, m, t);
	switch (res)
		{
			case SUCCESS_:
				r1 = r1_solver (a, x, b, n);
				r2 = r2_solver (x, n);
				time = (clock() - time) / CLOCKS_PER_SEC;
				printf("x:\n");
				print_matrix(x, 1, n, p);
				printf ("%s : Task = %d Res1 = %e Res2 = %e Elapsed = %.2f\n", argv[0], task, r1, r2, time);
				break;
			case ERROR_MATRIX:
				printf("Matrix A does not have D^-1\n");
				break;
			case ERROR_VECTOR:
				printf("Vector X is undefine\nd");
				break;
		}
	free(a);
    free(x);
    free(x0);
    free(b);
	return 0;
}
