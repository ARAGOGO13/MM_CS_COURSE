#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include "array_io.h"
#include "solve.h"

int main(int argc, char *argv[])
{
	int c, p, n, diff;
	int task = 5;
	char *filename;
	char **a;
	int (*cmp) (const char*, const char*);
	int (*f [])  (const char*, const char*) = {up_strcmp, down_strcmp, up_len, down_len};
	double t = 0;
	io_status ret;

	if (!(argc == 5 && sscanf(argv[1], "%d", &c) && sscanf(argv[2], "%d", &n) && sscanf(argv[3], "%d", &p) && c <= 4))
		{
			printf("Usage: %s c n p filename\n", argv[0]);
			return 1;
		}

	filename = argv[4];
	cmp = f[c - 1];

	if (!(a = (char **) malloc(n * sizeof(char *))))
		{
			printf("Can not allocate array len %d", n);
			return 2;
		}

	ret = read_array(a, n, filename);

	do
		{
			switch (ret)
				{
					case SUCCESS:
						continue;
					case ERROR_OPEN:
						printf("Can not open %s\n", filename);
						break;
					case ERROR_READ:
						printf("Can not read from %s\n", filename);
						break;
					case ERROR_MEM:
						printf("Not enough memory");
						break;
				}
			free(a);
			return 3;
	
		} while (0);

	print_array(a, n, p);

	t = clock();
	find_min_sort (a, n, cmp);
	t = (clock() - t) / CLOCKS_PER_SEC;

	diff = check (a, n, cmp);

	printf("New array:\n");
	print_array(a, n, p);
	printf ("%s : Task = %d Diff = %d Elapsed = %.2f\n", argv[0], task, diff, t);

	free_array(a, n);
	free(a);
	return 0;
}

