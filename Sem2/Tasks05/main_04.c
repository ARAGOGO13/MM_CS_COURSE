#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include "array_io.h"
#include "solve.h"

int main(int argc, char *argv[])
{
	double *a, *vec_b, *vec_x;
	int n, m, p, ka, kb, kx;
	char *filename_a = 0, *filename_b = 0, *filename_x = 0;
	double t = 0;
	double res = 0;
	int task = 4;

	if (!((7 <= argc && argc <= 10)
			&& sscanf(argv[1], "%d", &n) == 1
			&& sscanf(argv[2], "%d", &m) == 1
			&& sscanf(argv[3], "%d", &p) == 1
			&& sscanf(argv[4], "%d", &ka) == 1
			&& ka >= 0 && ka <= 4))
		{
      printf("Usage: %s n m p ka fa kb fb kx fx\n", argv[0]);
			return 0;
		}
    if (ka == 0)
      {
      	if (argc < 8)
      		{
      			printf("Usage: %s n m p ka fa kb fb kx fx\n", argv[0]);
            return 0;
      		}
        filename_a = argv[5];
        if (sscanf(argv[6], "%d", &kb) != 1)
          {
            printf("Usage: %s n m p ka fa kb fb kx fx\n", argv[0]);
            return 0;
          }
        if (kb == 0) 
        	{
                if (argc < 9)
                    {
                        printf("Usage: %s n m p ka fa kb fb kx fx\n", argv[0]);
                        return 0;
                    }
        		filename_b = argv[7];
        		if (sscanf(argv[8], "%d", &kx) != 1)
        			{
        				printf("Usage: %s n m p ka fa kb fb kx fx\n", argv[0]);
            		return 0;
        			}
        		if (kx == 0)
        			{
                        if (argc < 10)
                            {
                                printf("Usage: %s n m p ka fa kb fb kx fx\n", argv[0]);
                                return 0;
                            }
        				filename_x = argv[9];
        			}
        		if (kx != 0 && argc == 10)
        			{
        				printf("Usage: %s n m p ka fa kb fb kx fx\n", argv[0]);
            		return 0;
        			}
        	}
        else
        	{
        		if (sscanf(argv[7], "%d", &kx) != 1)
        			{
        				printf("Usage: %s n m p ka fa kb fb kx fx\n", argv[0]);
            		return 0;
        			}
        		if (kx == 0)
        			{
                        if (argc < 9)
                            {
                                printf("Usage: %s n m p ka fa kb fb kx fx\n", argv[0]);
                                return 0;
                            }
        				filename_x = argv[8];
        			}
        		if (kx != 0 && argc == 9)
        			{
        				printf("Usage: %s n m p ka fa kb fb kx fx\n", argv[0]);
            		return 0;
        			}
        	}
      }
    else
      {
        if (sscanf(argv[5], "%d", &kb) != 1)
          {
            printf("Usage: %s n m p ka fa kb fb kx fx\n", argv[0]);
            return 0;
          }
        if (kb == 0)
        	{
        		if (argc < 8)
          		{
          			printf("Usage: %s n m p ka fa kb fb kx fx\n", argv[0]);
                return 0;
          		}
          	filename_b = argv[6];
          	if (sscanf(argv[7], "%d", &kx) != 1)
        			{
        				printf("Usage: %s n m p ka fa kb fb kx fx\n", argv[0]);
            		return 0;
        			}
        		if (kx == 0)
        			{
                        if (argc < 9)
                            {
                                printf("Usage: %s n m p ka fa kb fb kx fx\n", argv[0]);
                                return 0;
                            }
        				filename_x = argv[8];
        			}
        		if (kx != 0 && argc == 9)
        			{
        				printf("Usage: %s n m p ka fa kb fb kx fx\n", argv[0]);
            		return 0;
        			}
        	}
        else
        	{
        		if (sscanf(argv[6], "%d", &kx) != 1)
        			{
        				printf("Usage: %s n m p ka fa kb fb kx fx\n", argv[0]);
            		return 0;
        			}
        		if (kx == 0)
        			{
                        if (argc < 8)
                            {
                                printf("Usage: %s n m p ka fa kb fb kx fx\n", argv[0]);
                                return 0;
                            }
        				filename_x = argv[7];
        			}
        		if (kx != 0 && argc > 7)
        			{
        				printf("Usage: %s n m p ka fa kb fb kx fx\n", argv[0]);
            		return 0;
        			}
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

	vec_x = (double *) malloc (m * sizeof(double));
	if (!vec_x)
		{
			free(a);
			printf("Not enough memory\n");
			return 2;
		}
	if (filename_x)
		{
			io_status ret;
			ret = read_matrix(vec_x, m, 1, filename_x);
			do
				{
					switch (ret)
						{
							case SUCCESS:
								continue;
							case ERROR_OPEN:
								printf("Can not open %s\n", filename_x);
								break;
							case ERROR_READ:
								printf("Can not read from %s\n", filename_x);
								break;
							case ERROR_MEM:
								printf("Not enough memory");
								break;
						}
					free(a);
          free(vec_x);
					return 3;
			
				} while (0);
		}
	else
		{
			init_matrix(vec_x, m, 1, kx);
		}



	vec_b = (double *) malloc (n * sizeof(double));
	if (!vec_b)
		{
			free(a);
			free(vec_x);
			printf("Not enough memory\n");
			return 2;
		}
	if (filename_b)
		{
			io_status ret;
			ret = read_matrix(vec_b, n, 1, filename_b);
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
                    free(vec_x);
                    free(vec_b);
					return 3;
			
				} while (0);
		}
	else
		{
			init_matrix(vec_b, n, 1, kb);
		}

	printf("Initial matrix A:\n");
	print_matrix(a, n, m, p);

	printf("Initial vector X:\n");
	print_matrix(vec_x, m, 1, p);

	printf("Initial vector B:\n");
	print_matrix(vec_b, n, 1, p);

	t = clock();
	res = residual_vector_max (a, vec_x, vec_b, n, m);
	t = (clock() - t) / CLOCKS_PER_SEC;

	printf ("%s : Task = %d Result = %e Elapsed = %.2f\n", argv[0], task, res, t);

	free(a);
  free(vec_x);
  free(vec_b);
	return 0;
}
