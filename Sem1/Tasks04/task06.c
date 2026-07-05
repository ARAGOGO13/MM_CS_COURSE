#include <stdio.h>

#define ERROR_OPEN (-1)
#define ERROR_READ (-2)
#define EMPTY_FILE (-3)

int find_cnt_constantly(double*, const char*);

int find_cnt_constantly(double *x, const char *file_name)
{
	FILE *f;	
	double cur;
	double previous;
	int cnt = 1;
	int cnt_of_const = 0;
	double mean_sum = 0, mean_cnt = 0;
	f = fopen(file_name, "r");

	if (!f) return ERROR_OPEN;

	if (fscanf(f, "%lf", &previous) != 1) 
		{
			if (feof(f)) 
				{
					fclose(f);
					return EMPTY_FILE;
				}
			fclose(f);
			return ERROR_READ;
		}

	while (fscanf(f, "%lf", &cur) == 1)
		{	
			if (cur <= previous && previous <= cur) 
				{
					cnt ++;
					mean_sum += previous;
					mean_cnt ++;
				}
			else 
				{
					if (cnt > 1)
						{
							cnt_of_const ++;
							mean_sum += previous;
							mean_cnt ++;
						}
					cnt = 1;
				}
			previous = cur;
		}

	if (cnt > 1)
		{
			cnt_of_const ++;
			mean_sum += previous;
			mean_cnt ++;
		}

	if (mean_cnt > 0) *x = mean_sum / mean_cnt;

	if (!feof(f)) 
		{	
			fclose(f);
			return ERROR_READ;
		}

	fclose(f);
	return cnt_of_const;
}

int main(int argc, char* argv[]) 
{
	int res;
	char *file_name;
	double x = 0;
	if (argc != 2)
		{	
			printf("Can not read from %s \n", argv[0]);
			return 1;
		}
	file_name = argv[1];
	res = find_cnt_constantly(&x, file_name);
	if (res >= 0) 
		{ 
    	printf("%s : Task = %d Result = %d X = %f\n", argv[0], 6, res, x);
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