#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include "solve.h"

int main(int argc, char *argv[])
{
	int task = 8, len = 0;
	int p = 0, ret = 0;
	char *filename = 0;
	double t = 0;
	node *head = 0;

	if (!((argc == 3) && sscanf(argv[1], "%d", &p) == 1))
		{
			printf("Usage: %s p file \n", argv[0]);
			return 0;
		}
	filename = argv[2];

	ret = read_list(filename, &head);
	do
		{
			switch (ret)
				{
					case 0:
						continue;
					case -1:
						printf("Can not open %s\n", filename);
						break;
					case -2:
						printf("Can not read from %s\n", filename);
						break;
					case -3:
						printf("Not enough memory");
						break;
				}
			delete_list (head);
			return 3;
		
		} while (0);

	printf("Initial list: \n");
	print_list (head, p);

	t = clock();
	head = task08 (head);
	t = (clock() - t) / CLOCKS_PER_SEC;

	printf("Result list:\n");
	print_list (head, p);
	len = get_length (head);
	delete_list (head);

	printf ("%s : Task = %d Length = %d Elapsed = %.2f\n", argv[0], task, len, t);
	return 0;
}
