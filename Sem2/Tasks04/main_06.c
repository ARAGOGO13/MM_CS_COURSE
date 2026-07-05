#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include "array_io.h"
#include "solve.h"

int main(int argc, char *argv[])
{
	double *a;
	int i, j, n, m, p, k;
	char *filename = 0;
	double t = 0;
	int task = 6;

	if (!((argc == 7 || argc == 8)
			&& sscanf(argv[1], "%d", &i) == 1
			&& sscanf(argv[2], "%d", &j) == 1
			&& sscanf(argv[3], "%d", &n) == 1
			&& sscanf(argv[4], "%d", &m) == 1
			&& sscanf(argv[5], "%d", &p) == 1
			&& sscanf(argv[6], "%d", &k) == 1
			&& k >= 0 &&k <= 4
			&& ((k == 0) == (argc == 8))))
		{
			printf("Usage: %s i j n m p k [name] \n", argv[0]);
			return 0;
		}
	if (argc == 8) filename = argv[7];
    
    if (i > n || j > n)
        {
                printf("Wrong i or j\n");
                return 4;
        }


	a = (double *) malloc (n * m * sizeof(double));
	if (!a)
		{
			printf("Not enough memory\n");
			return 2;
		}
	if (filename)
		{
			io_status ret;
			ret = read_matrix(a, n, m, filename);
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
					return 3;
			
				} while (0);
		}
	else
		{
			init_matrix(a, n, m, k);
		}
	printf("Initial matrix:\n");
	print_matrix(a, n, m, p);

	t = clock();
	el_row_operation_2 (a, n, m, i, j);
	t = (clock() - t) / CLOCKS_PER_SEC;

	printf("Result matrix:\n");
	print_matrix(a, n, m, p);
	printf ("%s : Task = %d Elapsed = %.2f\n", argv[0], task, t);

	free(a);
	return 0;
}
