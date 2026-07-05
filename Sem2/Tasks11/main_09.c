#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include "solve.h"
#include <math.h>

int main(int argc, char *argv[])
{
	double t = 0, a, b, eps;
	int n, k;
	int task = 9;
	double res = 0;
	double (*f) (double x);

	double (*funcs[7])(double) = { f0, f1, f2, f3, f4, f5, f6 };

	if (!((argc == 5)
			&& sscanf(argv[1], "%lf", &a) == 1
			&& sscanf(argv[2], "%lf", &b) == 1
			&& sscanf(argv[3], "%lf", &eps) == 1
			&& sscanf(argv[4], "%d", &k) == 1 && k >= 0 && k <= 6))
		{
			printf("Usage: %s a b eps k \n", argv[0]);
			return 0;
		}

	f = funcs[k];

	t = clock();
	n = integral_simpson_step (f, a, b, eps, &res);
	t = (clock() - t) / CLOCKS_PER_SEC;

	if (n == -1)
		{
			printf("Error: the integral does not converge\n");
			return 1;
		}

	printf ("%s : Task = %d Res = %e N = %d Count = %d T = %.2f\n", argv[0], task, res, n, get_count(), t);	
	return 0;
}
