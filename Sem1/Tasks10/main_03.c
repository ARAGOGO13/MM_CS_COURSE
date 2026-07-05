#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "test.h"
#include "solve.h"

int main(int argc, char *argv[])
{
	ssize_t n;
	char *s = 0;
	int ch = 0;
	char *res;
	double t;
	if (argc != 4 || sscanf(argv[1], "%zd", &n) != 1)
		{
			printf("Usage %s n str ch \n", argv[0]);
			return 1;
		}

	ch = (int) *argv[3];
	s = argv[2];

	t = test_3_4 (n, &strchr_, s, ch, &res);
	if (res != 0) printf("%s : Task = %d Res = %s Elapsed = %.2f\n", argv[0], 3, res, t);
	else printf("%s : Task = %d Res_std = Not found Elapsed = %.2f\n", argv[0], 3, t);

	t = test_3_4 (n, &strchr, s, ch, &res);
	if (res != 0) printf("%s : Task = %d Res_std = %s Elapsed = %.2f\n", argv[0], 3, res, t);
	else printf("%s : Task = %d Res_std = Not found Elapsed = %.2f\n", argv[0], 3, t);

	return 0;
}
