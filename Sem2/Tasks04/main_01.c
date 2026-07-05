#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include "array_io.h"
#include "solve.h"

int main(int argc, char *argv[])
{
	double *a;
	int n, p, k;
	char *filename = 0;
	double t = 0;
	int task = 1, res = 0;

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
	res = is_symmetric (a, n);
	t = (clock() - t) / CLOCKS_PER_SEC;

	printf("Result = %d\n", res);
	printf ("%s : Task = %d Elapsed = %.2f\n", argv[0], task, t);

	free(a);
	return 0;
}
