#include <stdio.h>
#include <time.h>
#include "solve.h"

int main (int argc, char *argv[])
{
	int task = 3;
	io_status ret;
	const char * f_in = 0;
	const char * f_out = 0;
	const char * s = 0;
	const char * t = 0;
	int res = 0;
	double time;

	if (argc != 5)
		{
			printf("Usage: %s <f_in> <f_out> <s>\n", argv[0]);
			return 1;
		}

	f_in = argv[1];
	f_out = argv[2];
	s = argv[3];
	t = argv[4];
	time = clock();
	ret = task03 (f_in, f_out, s, t, &res);
	time = (clock() - time) / CLOCKS_PER_SEC;

	switch (ret)
		{
			case SUCCESS:
				printf ("%s : Task = %d Result = %d Elapsed = %.2f\n",
				argv[0], task, res, time);
				break;
			case ERROR_OPEN:
				printf ("Can not open %s\n", f_out);
				return 1;
			case ERROR_READ:
				printf ("Can not read %s\n", f_out);
				return 2;
			case ERROR_PATTERN:
				printf ("Error in pattern %s\n", s);
				return 3;
		}

	return 0;
}


