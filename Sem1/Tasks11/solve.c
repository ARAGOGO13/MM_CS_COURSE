#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "solve.h"
#include "string_io.c"

int task01(char *filename_a, char *filename_b, char *s)
{
	int count = 0;
	char buff[LEN];
	FILE *file_a;
	FILE *file_b;

	if (!(file_a = fopen(filename_a, "r"))) return ERROR_OPEN;
	if (!(file_b = fopen(filename_b, "w"))) 
	{
		fclose(file_a);
		return ERROR_OPEN;
	}

	while (fgets(buff, LEN, file_a))
		{
			if (strcasestr_(buff, s)) 
				{
					fprintf(file_b, "%s", buff);
					count ++;
				}
		}

	if (!feof(file_a))
		{
			fclose(file_a);
			fclose(file_b);
			return ERROR_READ;
		}

	fclose(file_b);
	fclose(file_a);
	return count;
}

int task02(char *filename_a, char *filename_b, char *s)
{
	int count = 0;
	char buff[LEN];
	size_t len_s = strlen(s);
	size_t len_buff = 0;
	FILE *file_a;
	FILE *file_b;

	if (!(file_a = fopen(filename_a, "r"))) return ERROR_OPEN;
	if (!(file_b = fopen(filename_b, "w"))) 
	{
		fclose(file_a);
		return ERROR_OPEN;
	}

	while (fgets(buff, LEN, file_a))
		{
			len_buff = strlen(buff) - 1;
			if (strncmp_(buff + len_buff - len_s, s, len_s) == 0)
				{
					fprintf(file_b, "%s", buff);
					count ++;
				}
		}
	if (!feof(file_a))
		{
			fclose(file_a);
			fclose(file_b);
			return ERROR_READ;
		}

	fclose(file_b);
	fclose(file_a);
	return count;
}


int task03(char *filename_a, char *filename_b, char *s, char *t)
{
	int count = 0, flag = 0;
	char buff[LEN] = {0};
	char *pos;
	char *next;
	size_t len_t = 0, len_s = 0, len = 0;
	char result[LEN] = {0};
	FILE *file_a;
	FILE *file_b;

	len_t = strlen(t);
	len_s = strlen(s);

	result[0] = 0;

	if (len_s == 0) 
		{
			return EMPTY_STRING;
		}

	if (!(file_a = fopen(filename_a, "r"))) return ERROR_OPEN;
	if (!(file_b = fopen(filename_b, "w"))) 
	{
		fclose(file_a);
		return ERROR_OPEN;
	}

	while (fgets(buff, LEN, file_a)) 
		{
      pos = buff;
      flag = 0;
      while ((next = strstr(pos, s)) != 0) 
        {
        	len = next - pos;
          strncat_(result, pos, len);
          strncat_(result + len, t, len_t);
          pos = next + len_s;
          flag = 1;
        }
      strcat(result, pos);
  		if (flag) count++;
  		fprintf(file_b, "%s", result);
      result[0] = 0; 
		}

  if (!feof(file_a))
		{
			fclose(file_a);
			fclose(file_b);
			return ERROR_READ;
		}
	fclose(file_b);
	fclose(file_a);
	return count;
}

int task04(char *filename_a, char *filename_b, char *s, char *t)
{
	char buff[LEN] = {0};
	char purified_buff[LEN] = {0};
	char purified_s[LEN] = {0};
	size_t len_buff = 0, len_s = 0;
	int count = 0;
	size_t i = 0, j = 0;


	FILE *file_a;
	FILE *file_b;

	if (!(file_a = fopen(filename_a, "r"))) return ERROR_OPEN;
	if (!(file_b = fopen(filename_b, "w"))) 
	{
		fclose(file_a);
		return ERROR_OPEN;
	}


	while (fgets(buff, LEN, file_a)) 
		{
			i = 0;
      j = 0;
			purified_buff[0] = 0;

			len_buff = strlen(buff);
			for (i = 0; i < len_buff; i++)
				{
					if (!strchr(t, buff[i]))
						{
							purified_buff[j] = buff[i];
							j++;
						}
				}
			purified_buff[j - 1] = 0;

			j = 0;
			purified_s[0] = 0;

			len_s = strlen(s);
			for (i = 0; i < len_s; i++)
				{
					if (!strchr(t, s[i]))
						{
							purified_s[j] = s[i];
							j++;
						}
				}
			purified_s[j] = 0;

      if (strcmp(purified_buff, purified_s) != 0)
      	{
      		count ++;
      		fprintf(file_b, "%s", buff);
      	}
		}

  if (!feof(file_a))
		{
			fclose(file_a);
			fclose(file_b);
			return ERROR_READ;
		}

	fclose(file_b);
	fclose(file_a);
	return count;
}

int task05(char *filename_a, char *filename_b, char *s, char *t)
{
	char buff[LEN] = {0};
	int count = 0;
	size_t i = 0;
	int flag = 1;

	FILE *file_a;
	FILE *file_b;

	if (!(file_a = fopen(filename_a, "r"))) return ERROR_OPEN;
	if (!(file_b = fopen(filename_b, "w"))) 
	{
		fclose(file_a);
		return ERROR_OPEN;
	}


	while (fgets(buff, LEN, file_a)) 
		{
			i = 0;
			flag = 1;
      while (buff[i + 1] != 0)
      	{
      		if ((strchr(s, buff[i]) == 0) && (strchr(t, buff[i]) == 0)) 
      			{
      				flag = 0;
      				break;
      			}
      		i++;
      	}
      if (flag)
      	{
      		count ++;
      		fprintf(file_b, "%s", buff);
      	}
		}

  if (!feof(file_a))
		{
			fclose(file_a);
			fclose(file_b);
			return ERROR_READ;
		}
		
	fclose(file_b);
	fclose(file_a);
	return count;
}

int task06(char *filename_a, char *filename_b, char *s, char *t)
{
	char buff[LEN] = {0};
	int count = 0;
	size_t i = 0, len_buff = 0, j = 0, len_s = 0;
	char purified_buff[LEN] = {0};
	char purified_s[LEN] = {0};

	FILE *file_a;
	FILE *file_b;

	if (!(file_a = fopen(filename_a, "r"))) return ERROR_OPEN;
	if (!(file_b = fopen(filename_b, "w"))) 
	{
		fclose(file_a);
		return ERROR_OPEN;
	}

	while (fgets(buff, LEN, file_a)) 
		{
			j = 0;
			purified_buff[0] = 0;

			len_buff = strlen(buff);
			for (i = 0; i < len_buff; i++)
				{
					if (!strchr(t, buff[i]))
						{
							purified_buff[j] = buff[i];
							j++;
						}
				}
			purified_buff[j] = 0;

			j = 0;
			purified_s[0] = 0;

			len_s = strlen(s);
			for (i = 0; i < len_s; i++)
				{
					if (!strchr(t, s[i]))
						{
							purified_s[j] = s[i];
							j++;
						}
				}
			purified_s[j] = 0;

			if (strstr(purified_buff, purified_s))
				{
					fprintf(file_b, "%s", buff);
					count ++;
				}
		}

  if (!feof(file_a))
		{
			fclose(file_a);
			fclose(file_b);
			return ERROR_READ;
		}
		
	fclose(file_b);
	fclose(file_a);
	return count;
}

int task07(char *filename_a, char *filename_b, char *s, char *t)
{
	char buff[LEN] = {0};
	int count = 0;
	size_t i = 0;
	char *normal_s = 0;

	FILE *file_a;
	FILE *file_b;

	if (!(file_a = fopen(filename_a, "r"))) return ERROR_OPEN;
	if (!(file_b = fopen(filename_b, "w"))) 
	{
		fclose(file_a);
		return ERROR_OPEN;
	}

	while (fgets(buff, LEN, file_a)) 
		{
			i = 0;
			while (strchr(t, s[i]) && s[i]) i++;
			normal_s = s + i;
			if (strstr(buff, normal_s) != 0)
				{
					count ++;
					fprintf(file_b, "%s", buff);
				}
		}

  if (!feof(file_a))
		{
			fclose(file_a);
			fclose(file_b);
			return ERROR_READ;
		}
		
	fclose(file_b);
	fclose(file_a);
	return count;
}

int task08(char *filename_a, char *filename_b, char *s, char *l, char *r)
{
	int count = 0, flag = 0;
	char buff[LEN] = {0};
	char *pos = 0;
	char *next = 0;
	char t[LEN] = {0};
	char result[LEN] = {0};
	size_t len_t = 0, len_s = 0, len = 0;

	FILE *file_a;
	FILE *file_b;

	len_s = strlen(s);
	len_t = len_s + strlen(l) + strlen(r) + 1;


	result[0] = 0;

	if (len_s == 0) 
		{
			return EMPTY_STRING;
		}

	strcpy(t, l);
	strcat(t, s);
	strcat(t, r);
	len_t = strlen(t);

	if (!(file_a = fopen(filename_a, "r"))) return ERROR_OPEN;
	if (!(file_b = fopen(filename_b, "w"))) 
	{
		fclose(file_a);
		return ERROR_OPEN;
	}

	while (fgets(buff, LEN, file_a)) 
		{
      pos = buff;
      flag = 0;
      while ((next = strstr(pos, s)) != 0) 
        {
        	len = next - pos;
          strncat_(result, pos, len);
          strncat_(result + len, t, len_t);
          pos = next + len_s;
          flag = 1;
        }
      strcat(result, pos);
  		if (flag) count++;
  		fprintf(file_b, "%s", result);
      result[0] = 0; 
		}

  if (!feof(file_a))
		{
			fclose(file_a);
			fclose(file_b);
			return ERROR_READ;
		}
	fclose(file_b);
	fclose(file_a);
	return count;
}






























