#include <stdio.h>
#include "io_status.h"
#include "print_bits.h"

int print_bits (const char* filename)
{
	int x, i, bit;
	FILE* fp; 
	fp = fopen(filename, "r");

	if (!fp) return ERROR_OPEN;

	while (fscanf(fp, "%d", &x) == 1)
		{
			if (x == 0)
				{
					for (i = sizeof(int) * 8; i > 0; i--) printf("%d", 0);
				}
			i = sizeof(int) * 8 - 1;
      while (i >= 0 && !(x & (1 << i))) i--;
      for (; i >= 0; --i) 
      	{
          bit = (x >> i) & 1;
        	printf ("%d", bit);
        }
		}
	printf("\n");

	if (!feof(fp)) 
		{	
			fclose(fp);
			return ERROR_READ;
		}

	fclose(fp);
	return 0;
}

int print_bits0 (const char* filename)
{
	int x, i, bit;
	FILE* fp; 
	fp = fopen(filename, "r");

	if (!fp) return ERROR_OPEN;

	while (fscanf(fp, "%d", &x) == 1)
		{
			for (i = sizeof(int) * 8 - 1; i >= 0; --i) 
			{
				bit = (x >> i) & 1;
				printf ("%d", bit);
			}
		}
	printf("\n");

	if (!feof(fp)) 
		{	
			fclose(fp);
			return ERROR_READ;
		}

	fclose(fp);
	return 0;
}