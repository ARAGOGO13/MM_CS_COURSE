#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include "solve.h"
#include <math.h>

int main(int argc, char *argv[])
{
	double t = 0, a, eps;
	int k, b;
	int task = 10;
	double res = 0;
	double (*f) (double x);

	double (*funcs[7])(double) = { f0, f1, f2, f3, f4, f5, f6 };

	if (!((argc == 4)
			&& sscanf(argv[1], "%lf", &a) == 1
			&& sscanf(argv[2], "%lf", &eps) == 1
			&& sscanf(argv[3], "%d", &k) == 1 && k >= 0 && k <= 6))
		{
			printf("Usage: %s a eps k \n", argv[0]);
			return 0;
		}

	f = funcs[k];

	t = clock();
	b = integral_trapezoid_inf (f, a, eps, &res);
	t = (clock() - t) / CLOCKS_PER_SEC;

	if (b == -1)
		{
			printf("Error: the integral does not converge\n");
			return 1;
		}

	printf ("%s : Task = %d Res = %e B = %e Count = %d T = %.2f\n", argv[0], task, res, (double) b, get_count(), t);
	return 0;
}
