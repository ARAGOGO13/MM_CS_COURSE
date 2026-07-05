#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include "solve.h"
#include <math.h>

int main(int argc, char *argv[])
{
	double t = 0, a, b, eps;
	int kx, ky, r;
	int task = 12;
	double res = 0;
	double (*fx) (double x);
	double (*fy) (double x);

	double (*funcs[7])(double) = { f0, f1, f2, f3, f4, f5, f6 };

	if (!((argc == 6)
			&& sscanf(argv[1], "%lf", &a) == 1
			&& sscanf(argv[2], "%lf", &b) == 1
			&& sscanf(argv[3], "%lf", &eps) == 1
			&& sscanf(argv[4], "%d", &kx) == 1 && kx >= 0 && kx <= 6
			&& sscanf(argv[5], "%d", &ky) == 1 && ky >= 0 && ky <= 6))
		{
			printf("Usage: %s a b eps kx ky \n", argv[0]);
			return 0;
		}

	fx = funcs[kx];
	fy = funcs[ky];

	t = clock();
	r = curve_length (fx, fy, a, b, eps, &res);
	t = (clock() - t) / CLOCKS_PER_SEC;

	if (r == -1)
		{
			printf("Error: the integral does not converge\n");
			return 1;
		}

	printf ("%s : Task = %d Res = %e N = %d Count = %d T = %.2f\n", argv[0], task, res, r, get_count(), t);
	return 0;
}
