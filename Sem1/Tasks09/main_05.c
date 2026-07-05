#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "test.h"
#include "solve.h"

int main(int argc, char *argv[])
{
	ssize_t count;
	double t;
	char *s1_copy = 0;
	const char *s2;
	size_t len_s1, len_s2;
	if (argc != 4 || sscanf(argv[1], "%zd", &count) != 1)
		{
			printf("Usage: %s n s1 s2", argv[0]);
			return 1;
		}
	s2 = argv[3];
	len_s2 = strlen(argv[3]);

	len_s1 = strlen(argv[2]);
	s1_copy = (char *)malloc((len_s1 + len_s2 + 1) * sizeof(char));
	if (!s1_copy)
		{
			printf("Not enough memory! \n");
			return 3;
		}
	strcpy_(s1_copy, argv[2]);

	t = test_2_5(count, &strcat_, s1_copy, s2, &s1_copy);
	printf("%s : Task = %d Res = %s Elapsed = %.2f\n", argv[0], 5, s1_copy, t);
	strcpy_(s1_copy, argv[2]);

	t = test_2_5(count, &strcat, s1_copy, s2, &s1_copy);
	printf("%s : Task = %d Res_std = %s Elapsed = %.2f\n", argv[0], 5, s1_copy, t);
	free(s1_copy);
	
	return 0;
}
