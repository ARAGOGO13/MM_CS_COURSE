#include <stdio.h>

#define ERROR_OPEN (-1)
#define ERROR_READ (-2)
#define EMPTY_FILE (-3)

int find_max_element(double*, int*, int*, const char*);

int find_max_element(double *x, int *i, int *j, const char *file_name)
{
	int read_count = 1;
	FILE *f;	
	double cur;
	f = fopen(file_name, "r");

	if (!f) return ERROR_OPEN;

	if (fscanf(f, "%lf", x) != 1) 
		{
			if (feof(f)) 
				{
					fclose(f);
					return EMPTY_FILE;
				}
			fclose(f);
			return ERROR_READ;
		}

	*i = 1; *j = 1;

	while (fscanf(f, "%lf", &cur) == 1)
		{	
			read_count ++;
			if (cur > *x) 
				{	
					*x = cur;
					*i = read_count;
					*j = read_count;
				}
			else if (cur <= *x && *x <= cur) 
				{	
					*j = read_count;
				}
		}

	if (!feof(f)) 
		{	
			fclose(f);
			return ERROR_READ;
		}

	fclose(f);
	return 0;
}

int main(int argc, char* argv[]) 
{
	char *file_name;
	double x;
	int res, i, j;
	if (argc != 2)
		{	
			printf("Can not read from %s \n", argv[0]);
			return 1;
		}
	file_name = argv[1];
	res = find_max_element(&x, &i, &j, file_name);
	if (res >= 0) 
		{ 
    	printf("%s : Task = %d Result = %d X = %f i = %d j = %d\n", argv[0], 4, res, x, i, j);
    } 
  else 
   	{
      switch (res) 
	      {
	        case ERROR_OPEN:
	          printf("Error: Can not read from %s \n", file_name);
	          break;
	        case ERROR_READ:
	          printf("Error: Invalid character in the file %s \n", file_name);
	          break;
	        case EMPTY_FILE:
	          printf("Error: File %s is empty \n", file_name);
	          break;
	        default:
	          printf("Unknown error\n");
	           break;
	      }
    }
  return 0;
}