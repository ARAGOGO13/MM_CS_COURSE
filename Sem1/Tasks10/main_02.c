#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "test.h"
#include "solve.h"

int main(int argc, const char *argv[])
{
	ssize_t count;
	double t;
	char *s1;
	const char *s2;
	char *res = 0;
	if (argc != 3 || sscanf(argv[1], "%zd", &count) != 1)
		{
			printf("Usage: %s n str", argv[0]);
			return 1;
		}

	s2 = argv[2];
	s1 = (char *)malloc((strlen_(s2) + 1) * sizeof(char));
	if (!s1)
		{
			printf("Not enough memory \n");
			return 2;
		}
	s1[0] = 0;

	t = test_2_5(count, &strcpy_, s1, s2, &res);
	printf("%s : Task = %d Res = %s Elapsed = %.2f\n", argv[0], 2, res, t);

	t = test_2_5(count, &strcpy, s1, s2, &res);
	printf("%s : Task = %d Res_std = %s Elapsed = %.2f\n", argv[0], 2, res, t);

	free(s1);
	return 0;
}
