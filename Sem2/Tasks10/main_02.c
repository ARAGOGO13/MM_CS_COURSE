#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include "solve.h"
#include <math.h>

int main(int argc, char *argv[])
{
	double eps, x0;
	double t = 0, x = 0;
	int M, k;
	int task = 2;
	int it = 0;
	double fx;
	double (*f) (double x);
	double (*d) (double x);

	double (*funcs[7])(double) = { f0, f1, f2, f3, f4, f5, f6 };
	double (*diffs[7])(double) = { d0, d1, d2, d3, d4, d5, d6 };


	if (!((argc == 5)
			&& sscanf(argv[1], "%lf", &x0) == 1
			&& sscanf(argv[2], "%lf", &eps) == 1
			&& sscanf(argv[3], "%d", &M) == 1
			&& sscanf(argv[4], "%d", &k) == 1 && k >= 0 && k <= 6))
		{
			printf("Usage: %s a b eps M k \n", argv[0]);
			return 0;
		}

	f = funcs[k];
	d = diffs[k];

	t = clock();
	it = newton_method (f, d, x0, eps, M, &x);
	t = (clock() - t) / CLOCKS_PER_SEC;

	if (it == -1)
		{
			printf ("%s : Task = %d NOT FOUND Count = %d T = %.2f\n", argv[0], task, get_count(), t);
			return 0;
		}
	fx = f(x);
	printf ("%s : Task = %d X = %e Res = %e Its = %d Count = %d T = %.2f\n", argv[0], task, x, fx, it, get_count(), t);
	return 0;
}
