#include <stdio.h>

#define ERROR_OPEN (-1)
#define ERROR_READ (-2)

int find_first_element(double, const char*);

int find_first_element(double el, const char *file_name)
{
	double x;
	int read_count = 0;
	FILE *f;
	f = fopen(file_name, "r");

	if (!f) return ERROR_OPEN;

	while (fscanf(f, "%lf", &x) == 1)
		{	
			read_count ++;
			if (el <= x && x <= el) return read_count;
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
	double el;
	int res;
	if (argc != 3)
		{	
			printf("Can not read from %s \n", argv[0]);
			return 1;
		}
	file_name = argv[1];

	if (sscanf(argv[2], "%lf", &el) != 1) 
	{
		printf("Can not read from %s \n", argv[1]);
	}
	
	res = find_first_element(el, file_name);
	if (res >= 0) 
		{ 
    	printf("%s : Task = %d Result = %d\n", argv[0], 1, res);
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