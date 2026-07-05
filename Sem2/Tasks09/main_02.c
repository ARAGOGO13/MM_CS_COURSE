#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include "solve.h"

int main(int argc, char *argv[])
{
	double *x, *y;
	int n;
	char *filename = 0;
	double t = 0, x0 = 0;
	int task = 2;
	int res = 0;
	double result = 0;
	io_status ret;

	if (!((argc == 4)
			&& sscanf(argv[1], "%lf", &x0) == 1
			&& sscanf(argv[2], "%d", &n) == 1))
		{
			printf("Usage: %s n p k [name] \n", argv[0]);
			return 0;
		}
	filename = argv[3];

	x = (double *) malloc (n * sizeof(double));
	if (!x)
		{
			printf("Not enough memory\n");
			return 2;
		}
	y = (double *) malloc (n * sizeof(double));
	if (!y)
		{
			free(x);
			printf("Not enough memory\n");
			return 2;
		}

	ret = read_dots (x, y, n, filename);
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
			free(x);
			free(y);
			return 3;
	
		} while (0);

	t = clock();
	res = newton_interpolation (x0, x, y, n, &result);
	t = (clock() - t) / CLOCKS_PER_SEC;
	if (res == -1)
		{
			printf("Error: similar dots\n");
		}
	else
		{
			printf ("%s : Task = %d Result = %e Elapsed = %.2f\n", argv[0], task, result, t);
		}

	free(x);
	free(y);
	return 0;
}
