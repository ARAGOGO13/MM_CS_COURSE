#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "test.h"
#include "solve.h"

int main(int argc, char *argv[])
{
	ssize_t count;
	char *res;
	double t;
	char *s1;
	const char *s2;
	if (argc != 4 || sscanf(argv[1], "%zd", &count) != 1)
		{
			printf("Usage: %s n s1 s2", argv[0]);
			return 1;
		}

	s1 = argv[2];
	s2 = argv[3];

	t = test_9(count, &strstr_, s1, s2, &res);
	if (strlen_(res) != 0) printf("%s : Task = %d Res = %s Elapsed = %.2f\n", argv[0], 9, res, t);
	else printf("%s : Task = %d Res = Not found Elapsed = %.2f\n", argv[0], 9, t);

	t = test_9(count, &strstr, s1, s2, &res);
	if (res != 0) printf("%s : Task = %d Res_std = %s Elapsed = %.2f\n", argv[0], 9, res, t);
	else printf("%s : Task = %d Res_std = Not found Elapsed = %.2f\n", argv[0], 9, t);

	return 0;
}
