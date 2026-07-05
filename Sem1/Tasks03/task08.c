#include <stdio.h>
#include "task.h"

int type_of_sequence(const char*);

int type_of_sequence(const char *file_name)
{
	FILE *f;	
	double cur;
	double previous3, previous2, previous1;
	double d1, d2, q1 = 1, q2 = 1;
	int e1, e2;
	int status = 0;
	f = fopen(file_name, "r");

	if (!f) return ERROR_OPEN;

	if (fscanf(f, "%lf", &previous3) != 1) 
		{
			if (feof(f)) 
				{
					fclose(f);
					return 4;
				}
			fclose(f);
			return ERROR_READ;
		}

	if (fscanf(f, "%lf", &previous2) != 1) 
		{
			if (feof(f)) 
				{
					fclose(f);
					return 4;
				}
			fclose(f);
			return ERROR_READ;
		}

	if (fscanf(f, "%lf", &previous1) != 1) 
		{
			if (feof(f)) 
				{
					fclose(f);
					if (previous2 <= previous3 && previous2 >= previous3)	return 3;
					if ((previous2 <= 0 && previous2 >= 0) || (previous3 <= 0 && previous3 >= 0)) return 1;
					return 4;
				}
			fclose(f);
			return ERROR_READ;
		}


	d1 = previous2 - previous3;
	d2 = previous1 - previous2;

	if (!(previous2 <= 0 && previous2 >= 0) && !(previous3 <= 0 && previous3 >= 0))
		{	
			q1 = previous2 / previous3;
			q2 = previous1 / previous2;
		}


	e1 = previous2 <= previous3 && previous2 >= previous3;
	e2 = previous2 <= previous1 && previous2 >= previous1;

	if (d1 <= d2 && d1 >= d2 && !(d1 <= 0 && d1 >= 0)) status = 1;
	if (q1 <= q2 && q1 >= q2 && !(q1 <= 1 && q1 >= 1)) status = 2;
	if (e1 == e2 && e1 == 1) status = 3;

	while (fscanf(f, "%lf", &cur) == 1)
		{	
			d1 = previous1 - previous2;
			d2 = cur - previous1;

			if (!(previous2 <= 0 && previous2 >= 0) && !(previous1 <= 0 && previous1 >= 0))
				{
					q1 = previous1 / previous2;
					q2 = cur / previous1;
				}

			e1 = previous1 <= previous2 && previous1 >= previous2;
			e2 = previous1 <= cur && previous1 >= cur;

			if ((!(d1 <= d2 && d1 >= d2) && !(d1 <= 0 && d1 >= 0) && status == 1) || 
				  (!(q1 <= q2 && q1 >= q2) && !(q1 <= 1 && q1 >= 1) && status == 2) ||
				  (!(e1 == e2) && e1 == 1 && status == 3))
				{
					fclose(f);
					return 0;
				}
			previous2 = previous1;
			previous1 = cur;
		}

	if (!feof(f)) 
		{	
			fclose(f);
			return ERROR_READ;
		}

	fclose(f);
	return status;
} 



int main(int argc, char* argv[]) 
{
	int res;
	char *file_name;
	if (argc != 2)
		{	
			printf("Can not read from %s \n", argv[0]);
			return 1;
		}
	file_name = argv[1];
	res = type_of_sequence(file_name);
	if (res >= 0) 
		{ 
    	printf("%s : Task = %d Result = %d\n", argv[0], 8, res);
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