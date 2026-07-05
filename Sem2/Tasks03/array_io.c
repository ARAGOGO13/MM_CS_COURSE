#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "array_io.h"


io_status read_array (char **a, int n, const char *name)
{
	char buff[LEN] = {0};
	FILE *fp = 0;
	int i, j;

	if (!(fp = fopen(name, "r"))) return ERROR_OPEN;

	for (i = 0; i < n; i++)
		{
			if (!fgets(buff, sizeof(buff), fp))
				{
					fclose(fp);
					free_array (a, i);
					return ERROR_READ;
				}	
			for (j = 0; buff[j]; j++)
				{
					if (buff[j] == '\n')
						{
							buff[j] = 0;
							break;
						}
				}
			a[i] = (char *) malloc((j + 1) * sizeof(char));
			if (!a[i])
				{
					fclose(fp);
					free_array (a, i);
					return ERROR_MEM;
				}
			strcpy (a[i], buff);
		}

		fclose(fp);
		return SUCCESS;
}

void free_array (char **a, int n)
{
	int i;
	for (i = 0; i < n; i++)
		{
			if (a[i])
				{
					free(a[i]);
					a[i] = 0;
				}
		}
}

void print_array (char **a, int n, int m)
{
	int l = (n > m ? m : n);
	int i;
	for (i = 0; i < l; i++) printf("%s\n", a[i]);
}


int up_strcmp(const char *a, const char *b)
{
	return strcmp(a, b);
}

int down_strcmp(const char *a, const char *b)
{
	return -strcmp(a, b);
}

int up_len(const char *a, const char *b)
{
	ssize_t diff = (ssize_t) strlen(a) - (ssize_t) strlen(b);
	if (diff > 0) return 1;
	else if (diff < 0) return -1;
	return up_strcmp(a, b);
}

int down_len(const char *a, const char *b)
{
	return -up_len(a, b);
}

int check (char **a, int n, int (*cmp) (const char*, const char*))
{
	int i, count = 0;
	for (i = 1; i < n; i++)
		{
			if (cmp(a[i - 1], a[i]) > 0) count ++;
		}
	return count;
}


