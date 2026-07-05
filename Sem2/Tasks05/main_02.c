#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include "array_io.h"
#include "solve.h"

int main(int argc, char *argv[])
{
	double *a;
	int n, m, p, k;
	char *filename = 0;
	double t = 0;
	int task = 2;
	double res = 0;

        if (!((argc == 5 || argc == 6)
			&& sscanf(argv[1], "%d", &n) == 1
			&& sscanf(argv[2], "%d", &m) == 1
			&& sscanf(argv[3], "%d", &p) == 1
			&& sscanf(argv[4], "%d", &k) == 1
                        && k >= 0 && k <= 4
                        && ((k == 0) == (argc == 6))))
		{
			printf("Usage: %s n m p k [name] \n", argv[0]);
			return 0;
		}
	if (argc == 6) filename = argv[5];

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
	res = max_column_sum (a, n, m);
	t = (clock() - t) / CLOCKS_PER_SEC;

	printf ("%s : Task = %d Result = %e Elapsed = %.2f\n", argv[0], task, res, t);

	free(a);
	return 0;
}
