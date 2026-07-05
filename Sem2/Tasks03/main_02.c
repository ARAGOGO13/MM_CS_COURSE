#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include "array_io.h"
#include "solve.h"

int main(int argc, char *argv[])
{
	int cf, pa, pb, n, m, diff;
	int task = 2;
	char *filename_a, *filename_b;
	char **a, **b, **c;
	int (*cmp) (const char*, const char*);
	int (*f [])  (const char*, const char*) = {up_strcmp, down_strcmp, up_len, down_len};
	double t = 0;
	io_status ret;

	if (!(argc == 8 && sscanf(argv[1], "%d", &cf) &&
					 	sscanf(argv[2], "%d", &n) && 
					 	sscanf(argv[3], "%d", &pa) && 
					 	sscanf(argv[5], "%d", &m) &&
					 	sscanf(argv[6], "%d", &pb) &&
					 	cf <= 4))
		{
			printf("Usage: %s c n pa filename_a m pb filename_b\n", argv[0]);
			return 1;
		}

	filename_a = argv[4];
	filename_b = argv[7];
	cmp = f[cf - 1];

	if (!(a = (char **) malloc(n * sizeof(char *))))
		{
			printf("Can not allocate array len %d", n);
			return 2;
		}
	if (!(b = (char **) malloc(m * sizeof(char *))))
		{
			free(a);
			printf("Can not allocate array len %d", n);
			return 2;
		}
	if (!(c = (char **) malloc((n + m) * sizeof(char *))))
		{
			free(a);
			free(b);
			printf("Can not allocate array len %d", n);
			return 2;
		}

	ret = read_array(a, n, filename_a);
	do
		{
			switch (ret)
				{
					case SUCCESS:
						continue;
					case ERROR_OPEN:
						printf("Can not open %s\n", filename_a);
						break;
					case ERROR_READ:
						printf("Can not read from %s\n", filename_a);
						break;
					case ERROR_MEM:
						printf("Not enough memory");
						break;
				}
			free(a);
			return 3;
	
		} while (0);

	ret = read_array(b, m, filename_b);
	do
		{
			switch (ret)
				{
					case SUCCESS:
						continue;
					case ERROR_OPEN:
						printf("Can not open %s\n", filename_b);
						break;
					case ERROR_READ:
						printf("Can not read from %s\n", filename_b);
						break;
					case ERROR_MEM:
						printf("Not enough memory");
						break;
				}
			free(a);
			return 3;
	
		} while (0);

	printf("Array A:\n");
	print_array(a, n, pa);

	printf("Array B:\n");
	print_array(b, m, pb);

	t = clock();
	merge_arrays (a, b, c, n, m, cmp);
	t = (clock() - t) / CLOCKS_PER_SEC;

	printf("New array:\n");
	print_array(c, n + m, pa + pb);
	diff = check(a, n, cmp);

	printf("%s : Task = %d Diff = %d Elapsed = %.2f\n", argv[0], task, diff, t);

	free_array(a, n);
	free_array(b, m);

	free(a);
	free(b);
	free(c);

	return 0;
}

