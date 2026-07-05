#include <stdio.h>
int type_of_sequence(const char*);

int type_of_sequence(const char *file_name)
{
	double x, previous;
	int flag = 3, const_flag = 0;
	int read_count = 0;
	FILE *f;

	f = fopen(file_name, "r");

	if (!f) return -1;

	while (fscanf(f, "%lf", &x) == 1)
		{	
			read_count ++;
			if (read_count > 1) 
				{
					if (x > previous)
						{	
							if (flag == 2) flag = 0;
							else if (flag == 3) flag = 1;
						}
					else if (x < previous) 
						{	
							if (flag == 1) flag = 0;
							else if (flag == 3) flag = 2;
						}
				}
			previous = x;
		}

	if (read_count < 2) return 4;

	if (!feof(f)) 
		{	
			fclose(f);
			return -2;
		}

	fclose(f);
	if (const_flag) return 3;
	return flag;
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

	res = type_of_sequence(file_name);

	if (res >= 0) 
		{ 
    	printf("%s : Task = %d Result = %d\n", argv[0], 5, res);
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