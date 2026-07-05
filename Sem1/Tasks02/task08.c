#include <stdio.h>

int check_sequences(const char* filename_a, const char* filename_b);

int check_sequences(const char* filename_a, const char* filename_b) {
  double current_a, previous_a = 0, next_a = 0, middle_b = 0;
  int element_count = 0; 
  FILE* file_a;
  FILE* file_b;

  file_a = fopen(filename_a, "r");
  file_b = fopen(filename_b, "r");

  if (!file_a || !file_b) {
  	if (!file_a) fclose(file_b);
  	if (!file_b) fclose(file_a);
    return -1; 
  }

  if (fscanf(file_a, "%lf", &previous_a) != 1) 
	  {
	  	if (!feof(file_a)) 
	  	{
		  	fclose(file_a);
		  	fclose(file_b);
		  	return -2;
		  }
			fclose(file_a);
		 	fclose(file_b);
	  	return -3;
	  }

	if (fscanf(file_a, "%lf", &current_a) != 1) 
	  {
	  	if (!feof(file_a)) 
	  	{
		  	fclose(file_a);
		  	fclose(file_b);
		  	return -2;
		  }
			fclose(file_a);
		 	fclose(file_b);
	  	return -3;
	  }

	if (fscanf(file_b, "%lf", &middle_b) != 1) 
	  {
	  	fclose(file_a);
	  	fclose(file_b);
	  	return -3;
	  }



  while ((fscanf(file_a, "%lf", &next_a) == 1) && (fscanf(file_b, "%lf", &middle_b) == 1)) {
    element_count++;
    if (element_count > 1 && !(middle_b <= (previous_a + next_a) / 2 && middle_b >= (previous_a + next_a) / 2)) 
	    {
	      fclose(file_a);
			  fclose(file_b);
			  return 0; 
	    }

    previous_a = current_a;
    current_a = next_a;
  }

  if ((!feof(file_a)) && (!feof(file_b))) {
    fclose(file_a);
    fclose(file_b);
    return -2; 
  }

  fclose(file_a);
  fclose(file_b);

  if (element_count != 0) return 1;
  return -3; 
}

int main(int argc, char* argv[]) 
{
	char *filename_a, *filename_b;
	int res;
	if (argc != 3)
		{	
			printf("Can not read from %s \n", argv[0]);
			return 1;
		}
	filename_a = argv[1];
	filename_b = argv[2];

	res = check_sequences(filename_a, filename_b);

	if (res >= 0) 
		{ 
    	printf("%s : Task = %d Result = %d\n", argv[0], 8, res);
    } 

  else 
   	{
      switch (res) 
	      {
	        case -1:
	          printf("Error: Can not read from %s or %s \n", filename_a, filename_b);
	          break;
	        case -2:
	          printf("Error: Invalid character in the file %s or %s \n", filename_a, filename_b);
	          break;
	        case -3:
	        	printf("Error: Not enough data in the file %s or %s \n", filename_a, filename_b);
	          break;
	        default:
	          printf("Unknown error\n");
	           break;
	      }
    }
  return 0;
}