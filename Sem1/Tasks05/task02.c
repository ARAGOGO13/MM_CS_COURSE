#include <stdio.h>
#include "io_status.h"
#include "print_bits.h"

int count_leading_ones0(const char*, int*, int*);

int count_leading_ones0(const char* filename, int* i, int* j) {

  int count = -3, res = 0;
  int x;
  int first_one_index = -1;
  int last_one_index = -1;
  int bit_index, bit;
  int previous_bit1 = 0;
  int previous_bit2 = 0;


  FILE* fp = fopen(filename, "r");
  if (!fp) return ERROR_OPEN; 

  while (fscanf(fp, "%d", &x) == 1)
		{
      for (bit_index = sizeof(int) * 8 - 1; bit_index >= 0; --bit_index) 
      	{
      		count ++;
          bit = (x >> bit_index) & 1;
          if (previous_bit2 > previous_bit1 && previous_bit2 > bit)
	          {
	          	res ++;
	          	if (first_one_index == -1) first_one_index = count;
	          	last_one_index = count;
	          }
	        previous_bit2 = previous_bit1;
	        previous_bit1 = bit;
        }
		}

  if (!feof(fp)) 
	  {
	    fclose(fp);
	    return ERROR_READ;
	  }

  fclose(fp);

  *i = first_one_index;
  *j = last_one_index;

  return res;
}

int main(int argc, char* argv[]) 
{
	char *file_name;
	int res, i = -1, j = -1;
	if (argc != 2)
		{	
			printf("Can not read from %s \n", argv[0]);
			return 1;
		}

	file_name = argv[1];

	print_bits0(argv[1]);
	res = count_leading_ones0(file_name, &i, &j);

	if (res >= 0) 
		{ 
    	printf("%s : Task = %d Result = %d i = %d j = %d \n", argv[0], 2, res, i, j);
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