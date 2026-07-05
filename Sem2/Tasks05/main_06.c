#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include "array_io.h"
#include "solve.h"

int main(int argc, char *argv[])
{
	double *a, *b;
	int n, m, p, ka, kb;
	char *filename_a = 0, *filename_b = 0;
	double t = 0;
	double res = 0;
	int task = 6;

	if (!((6 <= argc && argc <= 8)
			&& sscanf(argv[1], "%d", &n) == 1
			&& sscanf(argv[2], "%d", &m) == 1
			&& sscanf(argv[3], "%d", &p) == 1
			&& sscanf(argv[4], "%d", &ka) == 1
			&& ka >= 0 && ka <= 4))
		{
            printf("Usage: %s n m k p ka fa kb fb \n", argv[0]);
			return 0;
		}
	if (ka == 0)
		{
			if (argc < 7)
	    		{
	    			printf("Usage: %s n m k p ka fa kb fb \n", argv[0]);
	                return 0;
	    		}
			filename_a = argv[5];
			if (sscanf(argv[6], "%d", &kb) != 1) 
				{
	                printf("Usage: %s n m k p ka fa kb fb \n", argv[0]);
					return 0;
				}
			if (kb == 0) filename_b = argv[7];
	        if (kb != 0 && argc == 8)
	            {
	                printf("Usage: %s n m k p ka fa kb fb \n", argv[0]);
	                return 0;
	            }
		}
	else
		{
			if (sscanf(argv[5], "%d", &kb) != 1) 
				{
                    printf("Usage: %s n m k p ka fa kb fb \n", argv[0]);
					return 0;
				}
            if (kb == 0) 
            	{
            		if (argc < 7)
                		{
                			printf("Usage: %s n m k p ka fa kb fb \n", argv[0]);
                            return 0;
                		}
		            filename_b = argv[6];
            	}
            if (kb != 0 && argc == 7)
                {
                    printf("Usage: %s n m k p ka fa kb fb \n", argv[0]);
                    return 0;
                }

		}

	a = (double *) malloc (n * m * sizeof(double));
	if (!a)
		{
			printf("Not enough memory\n");
			return 2;
		}
	if (filename_a)
		{
			io_status ret;
			ret = read_matrix(a, n, m, filename_a);
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
			init_matrix(a, n, m, ka);
		}

	b = (double *) malloc (m * n * sizeof(double));
	if (!b)
		{
			free(a);
			printf("Not enough memory\n");
			return 2;
		}
	if (filename_b)
		{
			io_status ret;
			ret = read_matrix(b, m, n, filename_b);
			do
				{
					switch (ret)
						{
							case SUCCESS:
								continue;
							case ERROR_OPEN:
								printf("Can not open %s\n", filename_b);
								break;
							case ERROR_READ:
								printf("Can not read from %s\n", filename_b);
								break;
							case ERROR_MEM:
								printf("Not enough memory");
								break;
						}
					free(a);
                                        free(b);
					return 3;
			
				} while (0);
		}
	else
		{
			init_matrix(b, m, n, kb);
		}


	printf("Initial matrix A:\n");
	print_matrix(a, n, m, p);

	printf("Initial matrix B:\n");
	print_matrix(b, m, n, p);

	t = clock();
	res = residual_matrix_column_sum (a, b, n, m);
	t = (clock() - t) / CLOCKS_PER_SEC;

	printf ("%s : Task = %d Result = %e Elapsed = %.2f\n", argv[0], task, res, t);

	free(a);
    free(b);
	return 0;
}
