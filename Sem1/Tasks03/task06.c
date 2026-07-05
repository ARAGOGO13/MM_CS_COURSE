#include <stdio.h>
int is_fib(const char*);

int is_fib(const char *file_name)
{
	double x, previous1, previous2;
	int read_count = 0;
	FILE *f;

	f = fopen(file_name, "r");

	if (!f) return -1;

	while (fscanf(f, "%lf", &x) == 1)
		{	
			read_count ++;
			if (read_count > 2 && (!(x <= previous1 + previous2 && x >= previous1 + previous2))) return 0;
			if (read_count == 2 && (!(x <= 1 && x >= 1 && previous1 <= 1 && previous1 >= 1))) return 0;
			previous2 = previous1;
			previous1 = x;
		}

	if (!feof(f)) 
		{	
			fclose(f);
			return -2;
		}

	if (read_count == 0) return 0;
	fclose(f);
	return 1;
}

int main(int argc, char* argv[]) 
{
	char *file_name;
	int res;
	if (argc != 2)
		{	
			printf("Can not read from %s \n", argv[0]);
			return 1;
		}
	file_name = argv[1];

	res = is_fib(file_name);

	if (res >= 0) 
		{ 
    	printf("%s : Task = %d Result = %d\n", argv[0], 6, res);
    } 

  else 
   	{
      switch (res) 
	      {
	        case -1:
	          printf("Error: Can not read from %s \n", file_name);
	          break;
	        case -2:
	          printf("Error: Invalid character in the file %s \n", file_name);
	          break;
	        default:
	          printf("Unknown error\n");
	           break;
	      }
    }
  return 0;
}