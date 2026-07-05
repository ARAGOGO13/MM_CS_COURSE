#include <stdio.h>
#include <math.h>

#define ERROR_OPEN (-1)
#define ERROR_READ (-2)
#define EMPTY_FILE (-3)
#define DIVISION_BY_ZERO (-4)

int mean_square_deviation(double*, const char*);

int mean_square_deviation(double *x, const char *file_name)
{
	FILE *f;	
	double cur;
	int cnt = 1;
	double sum = 0, sum_sq = 0;
	double mean;
	double tmp = 0;
	f = fopen(file_name, "r");

	if (!f) return ERROR_OPEN;

	if (fscanf(f, "%lf", &cur) != 1) 
		{
			if (feof(f)) 
				{
					fclose(f);
					return 4;
				}
			fclose(f);
			return ERROR_READ;
		}

	sum += cur;	
	while (fscanf(f, "%lf", &cur) == 1)
		{	
			sum += cur;
			sum_sq += cur * cur;
			cnt ++;
		}

	if (cnt < 1) return EMPTY_FILE;

	if (cnt == 1) {
		*x = 0;
		fclose(f);
		return 1;
	}

	mean = sum / cnt; 

	if (!feof(f)) 
		{	
			fclose(f);
			return ERROR_READ;
		}

	fclose(f);

	tmp = (sum_sq - 2 * mean * sum + cnt * mean * mean) / (cnt - 1);

	if (cnt == 1) *x = 0;
	if (tmp < 0) {
		return DIVISION_BY_ZERO;
	}

	*x = sqrt(tmp);
	return cnt;
}

int main(int argc, char* argv[]) 
{
	int res;
	char *file_name;
	double x;
	if (argc != 2)
		{	
			printf("Can not read from %s \n", argv[0]);
			return 1;
		}
	file_name = argv[1];
	res = mean_square_deviation(&x, file_name);
	if (res >= 0) 
		{ 
    	printf("%s : Task = %d Result = %d X = %f\n", argv[0], 7, res, x);
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
	        case DIVISION_BY_ZERO:
	          printf("Error: divivsion by zero \n");
	          break;
	        default:
	          printf("Unknown error\n");
	           break;
	      }
    }
  return 0;
}