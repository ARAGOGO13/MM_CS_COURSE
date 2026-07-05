#include <stdio.h>

#define ERROR_OPEN (-1)
#define ERROR_READ (-2)

int find_element(double, const char*, int*, int*);

int find_element(double el, const char *file_name, int *i, int *j)
{
	double x;
	int read_count = 0;
	FILE *f;
	int flag = 0;
	f = fopen(file_name, "r");

	if (!f) return ERROR_OPEN;

	while (fscanf(f, "%lf", &x) == 1)
		{	
			read_count ++;
			if (el <= x && x <= el) 
				{
					if (!flag) 
						{
							*i = read_count;
							flag = 1;
						}
					*j = read_count;
				}
		}

	if (!feof(f)) 
		{	
			fclose(f);
			return ERROR_READ;
		}

	fclose(f);
	if (*i > 0) return 1;
	return 0;
}

int main(int argc, char* argv[]) 
{
	char *file_name;
	double el;
	int res, i, j;
	if (argc != 3)
		{	
			printf("Can not read from %s \n", argv[0]);
			return 1;
		}
	if (sscanf(argv[1], "%lf", &el) != 1) 
	{
		printf("Can not read from %s \n", argv[1]);
	}
	file_name = argv[2];
	res = find_element(el, file_name, &i, &j);
	if (res >= 0) 
		{ 
    	printf("%s : Task = %d Result = %d i = %d j = %d\n", argv[0], 3, res, i, j);
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
	        default:
	          printf("Unknown error\n");
	           break;
	      }
    }
  return 0;
}