#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include "array_io.h"
#include "solve.h"


int main(int argc, char *argv[])
{	
	int task = 5;
	int n = 0, m = 0, l = 0;
	char *filename = 0, *s = 0;
	char **a = 0;
	double t;
	io_status ret;

	if (!(argc == 5 && sscanf(argv[1], "%d", &n) == 1 && sscanf(argv[2], "%d", &m) == 1))
		{
			printf("Usage: %s n m file\n", argv[0]);
			return 1;
		}
	filename = argv[3];
	s = argv[4];

	a = (char **) malloc(n * sizeof(char *));
	if (!a)
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
			free_array(a, n);
			free(a);
			return 3;
	
		} while (0);

	print_array(a, n, m);
	t = clock();
	l = task05 (a, n, s);
	t = (clock() - t) / CLOCKS_PER_SEC;

	printf("New array:\n");
	print_array(a, l, m);
	printf("%s : Task = %d Result = %d Elapsed = %.2f\n", argv[0], task, l, t);
	free_array(a, l);
	free(a);
	a = 0;

	return 0;
}



