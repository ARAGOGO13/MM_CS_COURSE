#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "test.h"
#include "solve.h"

int main(int argc, char *argv[])
{
	ssize_t count;
	char *next;
	double t;
	char *str;
	char *original_str = 0;
	char *res = 0;
	const char *delim;
	if (argc != 4 || sscanf(argv[1], "%zd", &count) != 1)
		{
			printf("Usage: %s n s1 s2", argv[0]);
			return 1;
		}


	str = argv[2];
	delim = argv[3];
	original_str = (char *)malloc((strlen_(str) + 1) * sizeof(char));
	if (!original_str)
		{
			printf("Not enough memory");
			return 3;
		}
	strcpy_(original_str, str);

	t = test_10(count, &strtok_r_, str, delim, &next, original_str, &res);
	if (res != 0) 
		{
			if (next == 0) printf ("%s : Task = %d Res = %s Saveptr =  Elapsed = %.2f\n", argv[0], 10, res, t);
			else printf ("%s : Task = %d Res = %s Saveptr = %s Elapsed = %.2f\n", argv[0], 10, res, next, t);
		}
	else 
		{
			if (next == 0) printf ("%s : Task = %d Res = Not found Saveptr =  Elapsed = %.2f\n", argv[0], 10, t);
			else printf ("%s : Task = %d Res = Not found Saveptr = %s Elapsed = %.2f\n", argv[0], 10, next, t);
		}

	t = test_10(count, &strtok_r, str, delim, &next, original_str, &res);
	if (res != 0) 
		{
			if (next == 0) printf ("%s : Task = %d Res_std = %s Saveptr =  Elapsed = %.2f\n", argv[0], 10, res, t);
			else printf ("%s : Task = %d Res_std = %s Saveptr = %s Elapsed = %.2f\n", argv[0], 10, res, next, t);
		}
	else 
		{
			if (next == 0) printf ("%s : Task = %d Res = Not found Saveptr =  Elapsed = %.2f\n", argv[0], 10, t);
			else printf ("%s : Task = %d Res_std = Not found Saveptr = %s Elapsed = %.2f\n", argv[0], 10, next, t);
		}

	free(original_str);
	return 0;
}
