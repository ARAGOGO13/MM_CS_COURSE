#include <stdio.h>
#include "io_status.h"
#include "print_bits.h"

int max_ones_sequence(const char*, int*, int*);

int max_ones_sequence(const char* filename, int* i, int* j) {

  int count = -1;
  int cnt_of_ones = 0;
  int x;
  int first_ones = -1;
  int last_ones = -1;
  int bit_index, bit;
  int flag = 0;


  FILE* fp = fopen(filename, "r");
  if (!fp) return ERROR_OPEN; 



  while (fscanf(fp, "%d", &x) == 1)
		{
			bit_index = sizeof(int) * 8 - 1;
			if (x != 0) 
			{
				while (bit_index >= 0 && !(x & (1 << bit_index))) bit_index--;
			}
      for (; bit_index >= 0; --bit_index) 
      	{
      		count ++;
          bit = (x >> bit_index) & 1;
          if (bit)
          	{
          		if (flag == 0) 
          			{
          				if (first_ones == -1) first_ones = count;
          				last_ones = count;
          				cnt_of_ones ++;
          			}
          		flag = 1;
          	}
          if (!bit) 
          	{
          		flag = 0;
          	}
        }
		}

  if (!feof(fp)) 
	  {
	    fclose(fp);
	    return ERROR_READ;
	  }

  fclose(fp);

  if (count == -1)
  	{	
  		first_ones = -1;
  		last_ones = -1;
  	}

  *i = first_ones;
  *j = last_ones;

  return cnt_of_ones;
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

	print_bits(argv[1]);
	res = max_ones_sequence(file_name, &i, &j);

	if (res >= 0) 
		{ 
    	printf("%s : Task = %d Result = %d i = %d j = %d \n", argv[0], 7, res, i, j);
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