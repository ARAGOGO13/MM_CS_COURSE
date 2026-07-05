#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "test.h"
#include "solve.h"

int main(int argc, char *argv[])
{
	ssize_t count;
	double t;
	const char *s1 = 0;
	const char *s2 = 0;
	int res;
	if (argc != 4 || sscanf(argv[1], "%zd", &count) != 1)
		{
			printf("Usage: %s n s1 s2", argv[0]);
			return 1;
		}
		
	s1 = argv[2];
	s2 = argv[3];

	t = test_6(count, &strcmp_, s1, s2, &res);
	printf("%s : Task = %d Res = %d Elapsed = %.2f\n", argv[0], 6, res, t);

	t = test_6(count, &strcmp, s1, s2, &res);
	printf("%s : Task = %d Res_std = %d Elapsed = %.2f\n", argv[0], 6, res, t);

	return 0;
}
