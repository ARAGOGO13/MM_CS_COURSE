#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "test.h"
#include "solve.h"

int main(int argc, char *argv[])
{
	ssize_t count;
	size_t res;
	double t;
	const char *s1 = 0;
	const char *s2 = 0;
	if (argc != 4 || sscanf(argv[1], "%zd", &count) != 1)
		{
			printf("Usage: %s n s1 s2", argv[0]);
			return 1;
		}

	s1 = argv[2];
	s2 = argv[3];

	t = test_7_8(count, &strspn_, s1, s2, &res);
	printf("%s : Task = %d Res = %zd Elapsed = %.2f\n", argv[0], 8, res, t);

	t = test_7_8(count, &strspn, s1, s2, &res);
	printf("%s : Task = %d Res_std = %zd Elapsed = %.2f\n", argv[0], 8, res, t);

	return 0;
}
