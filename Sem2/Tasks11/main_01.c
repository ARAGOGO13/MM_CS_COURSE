#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include "solve.h"
#include <math.h>

int main(int argc, char *argv[])
{
	double t = 0, x = 0, h = 0;
	int k;
	int task = 1;
	double dx;
	int res = 0;
	double (*f) (double x);

	double (*funcs[7])(double) = { f0, f1, f2, f3, f4, f5, f6 };

	if (!((argc == 4)
			&& sscanf(argv[1], "%lf", &x) == 1
			&& sscanf(argv[2], "%lf", &h) == 1
			&& sscanf(argv[3], "%d", &k) == 1 && k >= 0 && k <= 6))
		{
			printf("Usage: %s x h k \n", argv[0]);
			return 0;
		}

	f = funcs[k];

	t = clock();
	res = derivative01 (f, x, h, &dx);
	t = (clock() - t) / CLOCKS_PER_SEC;

	if (res == -1)
		{
			printf("Error: h is equal to zero\n");
			return 1;
		}

	printf ("%s : Task = %d Res = %e Count = %d T = %.2f\n", argv[0], task, dx, get_count(), t);	
	return 0;
}
