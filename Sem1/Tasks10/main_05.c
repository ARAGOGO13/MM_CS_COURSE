#include <stdio.h>
#include <time.h>
#include "solve.h"

int main(int argc, char *argv[])
{
	char *filename_a = 0;
	char *filename_b = 0;
	char *str = 0;
	char *t_str = 0;
	int result = 0;
	double t = 0;

	if (!(argc == 5)) 
		{
			printf("Usage: %s filename_a filename_b s", argv[0]);
			return -1;
		}

	filename_a = argv[1];
	filename_b = argv[2];
	str = argv[3];
	t_str = argv[4];

	t = clock();
	result = task05(filename_a, filename_b, str, t_str);
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
	printf ("%s : Task = %d Result = %d Elapsed = %.2f\n", argv[0], 5, result, t);
	return 0;
}
