#include <stdio.h>
#include <time.h>
#include "solve.h"

int main(int argc, char *argv[])
{
	char *filename_a = 0;
	char *filename_b = 0;
	char *str = 0;
	int result = 0;
	double t = 0;
	if (!(argc == 4)) 
		{
			printf("Usage: %s filename_a filename_b s", argv[0]);
			return -1;
		}

	filename_a = argv[1];
	filename_b = argv[2];
	str = argv[3];

	t = clock();
	result = task01(filename_a, filename_b, str);
	if (result < 0)
		{	
			switch (result) 
	      	{
		        case ERROR_OPEN:
		          printf("Error: Can not read from %s \n", filename_a);
		          break;
		        case ERROR_READ:
		          printf("Error: reading the file %s \n", filename_a);
		          break;
		        default:
		          printf("Unknown error\n");
		          break;
	      	}
	    	return 3;
		}
	t = (clock() - t) / CLOCKS_PER_SEC;
	printf ("%s : Task = %d Result = %d Elapsed = %.2f\n", argv[0], 1, result, t);
	return 0;
}
