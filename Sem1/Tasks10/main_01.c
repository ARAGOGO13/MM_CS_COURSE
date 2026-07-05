#include <stdio.h>
#include <string.h>
#include <time.h>
#include "test.h"
#include "solve.h"

int main (int argc, char *argv[])
{
	size_t len, len_std; ssize_t count;
	double t, t_std;
	char *s = 0;
	if (argc != 3 || sscanf(argv[1], "%zd", &count) != 1)
		{
			printf("Usage: %s n str \n", argv[0]);
			return 1;
		}

	s = argv[2];

	t = test_1(count, &strlen_, s, &len);
	printf("%s : Task = %d Res = %zd Elapsed = %.2f\n", argv[0], 1, len, t);

	t_std = test_1(count, &strlen, s, &len_std);
	printf("%s : Task = %d Res_std = %zd Elapsed = %.2f\n", argv[0], 1, len_std, t_std);
	
	return 0;
}
