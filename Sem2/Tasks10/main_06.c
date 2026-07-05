#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include "solve.h"
#include <math.h>

int main(int argc, char *argv[])
{
	double eps, a, b;
	double t = 0, x = 0;
	int M, k, m;
	int task = 6;
	int it = 0;
	double fx;
	double (*f) (double x);
	double *d;

	double (*funcs[7])(double) = { f0, f1, f2, f3, f4, f5, f6 };

	if (!((argc == 7)
			&& sscanf(argv[1], "%d", &m) == 1
			&& sscanf(argv[2], "%lf", &a) == 1
			&& sscanf(argv[3], "%lf", &b) == 1
			&& sscanf(argv[4], "%lf", &eps) == 1
			&& sscanf(argv[5], "%d", &M) == 1
			&& sscanf(argv[6], "%d", &k) == 1 && k >= 0 && k <= 6))
		{
			printf("Usage: %s a b eps M k \n", argv[0]);
			return 0;
		}

	d = (double *) malloc(3 * (m + 1) * sizeof(double));
	if (!d)
		{
			printf("Not enough memory\n");
			return 2;
		}

	f = funcs[k];

	t = clock();
	it = reverse_interpolation_method (f, m, d, a, b, eps, M, &x);

	t = (clock() - t) / CLOCKS_PER_SEC;

	if (it == -1)
		{
			printf ("%s : Task = %d NOT FOUND Count = %d T = %.2f\n", argv[0], task, get_count(), t);
			return 0;
		}

	fx = f(x);
	printf ("%s : Task = %d X = %e Res = %e Its = %d Count = %d T = %.2f\n", argv[0], task, x, fx, it, get_count(), t);
	free(d);
	return 0;
}
