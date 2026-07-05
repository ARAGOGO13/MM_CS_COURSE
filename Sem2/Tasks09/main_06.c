#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include "solve.h"
#include <math.h>

int main(int argc, char *argv[])
{
	double eps, r1, r2;
	double t = 0, x = 0;
	int task = 6;
	int res = 0;

	if (!((argc == 3)
			&& sscanf(argv[1], "%lf", &x) == 1
			&& sscanf(argv[2], "%lf", &eps) == 1))
		{
			printf("Usage: %s x eps \n", argv[0]);
			return 0;
		}

	t = clock();
	res = taylor_cos (x, eps, &r1);
	if (res == -2)
		{
			printf("eps can not be less then zero\n");
			return 1;
		}
	t = (clock() - t) / CLOCKS_PER_SEC;
	r2 = fabs(cos(x) - r1);
	printf ("%s : Task = %d Result = %e Residual = %e Elapsed = %.2f\n", argv[0], task, r1, r2, t);	
	return 0;
}
