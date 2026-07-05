#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include "array_io.h"
#include "solve.h"

int main(int argc, char *argv[])
{
	double *a, *b, *c;
	int n, m, p, ka, kb;
	char *filename_a = 0, *filename_b = 0;
	double t = 0;
	int task = 9;

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

	b = (double *) malloc (m * sizeof(double));
	if (!b)
		{
			free(a);
			printf("Not enough memory\n");
			return 2;
		}
	if (filename_b)
		{
			io_status ret;
			ret = read_matrix(b, m, 1, filename_b);
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
			init_matrix(b, m, 1, kb);
		}

        c = (double *) malloc (n * sizeof(double));
	if (!c)
		{
			free(a);
			free(b);
			printf("Not enough memory\n");
			return 2;
		}

	printf("Initial matrix A:\n");
	print_matrix(a, n, m, p);

	printf("Initial vector B:\n");
	print_matrix(b, m, 1, p);

	t = clock();
	matrix_vector_multiplication (a, b, m, c, n);
	t = (clock() - t) / CLOCKS_PER_SEC;

	printf("Result matrix:\n");
        print_matrix(c, n, 1, p);
	printf ("%s : Task = %d Elapsed = %.2f\n", argv[0], task, t);

	free(a);
        free(b);
        free(c);
	return 0;
}
