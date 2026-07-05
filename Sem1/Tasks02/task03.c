#include <stdio.h>
int cnt_of_max (const char*);

int cnt_of_max(const char *file_name)
{
	double x, mx = 0;
	int count = 1, read_count = 0;
	FILE *f;
	f = fopen(file_name, "r");

	if (!f) return -1;

	while (fscanf(f, "%lf", &x) == 1)
		{	
			read_count ++;
			if (read_count == 1) mx = x;
			if (mx <= x && mx >= x && read_count > 1) count ++;
			else if (x > mx) 
				{
					count = 1;
					mx = x;
				}
		}

	if (!feof(f)) 
		{	
			fclose(f);
			return -2;
		}

	fclose(f);
	return count;
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
	res = cnt_of_max(file_name);
	if (res >= 0) 
		{ 
    	printf("%s : Task = %d Result = %d\n", argv[0], 3, res);
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
	        case -3:
	          printf("Error: File is empty %s \n", file_name);
	          break;
	        default:
	          printf("Unknown error\n");
	           break;
	      }
    }
  return 0;
}