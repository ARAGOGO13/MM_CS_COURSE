#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "solve.h"

int words_div(const char *buff, const char *t, int *words_index) 
{
  size_t i;
  int j = 0;
  int in_word = 0;
  size_t len = strlen(buff);

  for (i = 0; i < len; i++) 
    {
      if (strchr(t, buff[i])) 
	      {
          if (in_word) 
	          {
              words_index[j++] = i - 1;
              in_word = 0;
	          }
	      } 
	    else if (!in_word)
        {
          words_index[j++] = i;
          in_word = 1;
        }
  	}
  if (in_word) words_index[j++] = len - 1;

  return j / 2;
}


io_status processing_s (const char *s, char *s1 ,char *s2, char ss, int flag)
{
	size_t i, j = 0;

	for (i = 0; s[i]; i++)
		{
			if (s[i] == '\\')
				{
					i++;
					if (s[i] && ((s[i] == '<' && ss == '<') || (s[i] == '>' && ss == '>')))
						{
							s1[j] = s[i];
							s2[j] = '1';
							j++;
						}
					else if (s[i])
						{
							s1[j] = s[i];
							s2[j] = '0';
							j++;
						} 
					else return ERROR_PATTERN;
				}
			else
				{
					s1[j] = s[i];
					if (s[i] == ss && ((flag == 0 && i == 0)
												|| (flag == 1 && i == strlen(s) - 1)
												|| (flag == 2))) s2[j] = '1';
					else s2[j] = '0';
					j++;
				}
		}
	s1[j] = '\0';
	s2[j] = '\0';
	return SUCCESS;
} 

io_status processing_s_7_9 (const char *s, char *s1 ,char *s2, char ss)
{
	size_t i;
	char temp_s1[LEN];
	char temp_s2[LEN];
	size_t j = 0;
	int start_flag = 0;

	for (i = 0; s[i]; i++) 
		{
			if (s[i] == '\\') 
				{
					i++;
					if (s[i]) 
						{
							temp_s1[j] = s[i];
							temp_s2[j] = '0';
							j++;
							start_flag = 0;
						}
					else return ERROR_PATTERN;
				} 
			else if (s[i] == ss) 
				{
					if (start_flag == 0)
						{
							if (j > 0)
								{
									temp_s2[j - 1] = '1';
								}
							else 
								{
									temp_s1[j] = s[i];
									temp_s2[j] = '0';
									j++;
								}
						}
					start_flag = 1;
					continue;
				} 
			else 
				{
					temp_s1[j] = s[i];
					temp_s2[j] = '0';
					j++;
					start_flag = 0;
				}
		}
	temp_s1[j] = '\0';
	temp_s2[j] = '\0';

	strcpy(s1, temp_s1);
	strcpy(s2, temp_s2);

  return SUCCESS;
} 

io_status processing_s_10 (const char *s, char *s1, char *s2)
{
	size_t i = 0;
	size_t j = 0;
	int escape_flag = 0, in_brackets_flag = 0;
	char start_ch = 0;

	for (i = 0; s[i]; i++)
		{
			if (escape_flag)
				{
					s2[j] = '0';
					s1[j] = s[i];
					j++;
					escape_flag = 0;
				}
			else if (in_brackets_flag)
				{
					if (s[i] == '-') start_ch = s[i - 1];
					else if (s[i] == ']')
						{
							s2[j] = '1';
							s1[j] = start_ch;
							j++;

							s2[j] = '1';
							s1[j] = s[i - 1];
							j++;

							in_brackets_flag = 0;
						}
					else if (s[i] == '[') return ERROR_PATTERN;
				}
			else
				{
					if (s[i] == '[') in_brackets_flag = 1;
					else if (s[i] == '\\')
						{
							if (i == strlen(s) - 1) return ERROR_PATTERN;
							escape_flag = 1;
						}
					else if (s[i] == ']') return ERROR_PATTERN;
					else
						{
							s2[j] = '0';
							s1[j] = s[i];
							j++;
						}
				}
		}
		if (in_brackets_flag || escape_flag) return ERROR_PATTERN;

		s1[j] = '\0';
		s2[j] = '\0';

		return SUCCESS;
}


io_status task01 (const char *filename_a, const char *filename_b, const char *s, int *r)
{

	int count = 0;
	char buff[LEN];
	char s1[LEN];
	char s2[LEN];
	io_status ret;
	FILE *file_a;
	FILE *file_b;

	if (!(file_a = fopen(filename_a, "r"))) return ERROR_OPEN;
	if (!(file_b = fopen(filename_b, "w"))) 
		{
			fclose(file_a);
			return ERROR_OPEN;
		}

	ret = processing_s(s, s1, s2, '^', 0);

	if (ret != SUCCESS) 
		{
			fclose(file_a);
			fclose(file_b);
			return ret;
		}

	while (fgets(buff, LEN, file_a))
		{
			buff[strlen(buff) - 1] = '\0';
			if (s2[0] == '1')
				{
					if (strncmp(buff, s1 + 1, strlen(s1) - 1) == 0)
						{
							fprintf(file_b, "%s", buff);
							count++;
						}
				}
			else
				{
					if (strstr(buff, s1))
						{
							fprintf(file_b, "%s", buff);
							count++;
						}
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

	*r = count;

	return SUCCESS;

}

io_status task02 (const char *filename_a, const char *filename_b, const char *s, int *r)
{

	int count = 0;
	char buff[LEN];
	char s1[LEN];
	char s2[LEN];
	io_status ret;
	FILE *file_a;
	FILE *file_b;

	if (!(file_a = fopen(filename_a, "r"))) return ERROR_OPEN;
	if (!(file_b = fopen(filename_b, "w"))) 
		{
			fclose(file_a);
			return ERROR_OPEN;
		}

	ret = processing_s(s, s1, s2, '$', 1);

	if (ret != SUCCESS) 
		{
			fclose(file_a);
			fclose(file_b);
			return ret;
		}

	while (fgets(buff, LEN, file_a))
		{
			buff[strlen(buff) - 1] = '\0';
			if (s2[strlen(s2) - 1] == '1')
				{
					if (strncmp(buff + strlen(buff) - strlen(s1) + 1, s1, strlen(s1) - 1) == 0)
						{
							fprintf(file_b, "%s\n", buff);
							count++;
						}
				}
			else
				{
					if (strstr(buff, s1))
						{
							fprintf(file_b, "%s\n", buff);
							count++;
						}
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

	*r = count;

	return SUCCESS;

}


io_status task03 (const char *filename_a, const char *filename_b, const char *s, const char *t, int *r)
{

	int count = 0, i = 0;
	int cnt_of_words = 0;
	char buff[LEN];
	char s1[LEN];
	char s2[LEN];
	char word[LEN];
	io_status ret;
	FILE *file_a;
	FILE *file_b;
  size_t len_word;
  int words_index[LEN];

	if (!(file_a = fopen(filename_a, "r"))) return ERROR_OPEN;
	if (!(file_b = fopen(filename_b, "w"))) 
		{
			fclose(file_a);
			return ERROR_OPEN;
		}

	ret = processing_s(s, s1, s2, '<', 0);

	if (ret != SUCCESS) 
		{
			fclose(file_a);
			fclose(file_b);
			return ret;
		}

	while (fgets(buff, LEN, file_a))
		{
			buff[strlen(buff) - 1] = '\0';
			cnt_of_words = words_div(buff, t, words_index);
			for (i = 0; i < cnt_of_words; i++)
				{
					strncpy(word, buff + words_index[2 * i], words_index[2 * i + 1] - words_index[2 * i] + 1);
					word[words_index[2 * i + 1] - words_index[2 * i] + 1] = '\0';
					len_word = words_index[2 * i + 1] - words_index[2 * i] + 1;
					if (s2[0] == '1')
						{
							if (strlen(s1) - 1 <= len_word && strncmp(word, s1 + 1, strlen(s1) - 1) == 0)
								{
									fprintf(file_b, "%s\n", buff);
									count++;
									break;
								}
						}
					else
						{
							if (strstr(word, s1))
								{
									fprintf(file_b, "%s\n", buff);
									count++;
									break;
								}
						}
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

	*r = count;

	return SUCCESS;

}

io_status task04 (const char *filename_a, const char *filename_b, const char *s, const char *t, int *r)
{

	int count = 0, i = 0;
	int cnt_of_words = 0;
	char buff[LEN];
	char s1[LEN];
	char s2[LEN];
	char word[LEN];
	io_status ret;
	FILE *file_a;
	FILE *file_b;
  size_t len_word;
  int words_index[LEN];

	if (!(file_a = fopen(filename_a, "r"))) return ERROR_OPEN;
	if (!(file_b = fopen(filename_b, "w"))) 
		{
			fclose(file_a);
			return ERROR_OPEN;
		}

	ret = processing_s(s, s1, s2, '>', 1);

	if (ret != SUCCESS) 
		{
			fclose(file_a);
			fclose(file_b);
			return ret;
		}

	while (fgets(buff, LEN, file_a))
		{
			buff[strlen(buff) - 1] = '\0';
			cnt_of_words = words_div(buff, t, words_index);
			for (i = 0; i < cnt_of_words; i++)
				{
					strncpy(word, buff + words_index[2 * i], words_index[2 * i + 1] - words_index[2 * i] + 1);
					word[words_index[2 * i + 1] - words_index[2 * i] + 1] = '\0';
					len_word = words_index[2 * i + 1] - words_index[2 * i] + 1;
					if (s2[strlen(s2) - 1] == '1')
						{
							if (strncmp(word + len_word - strlen(s1) + 1, s1, strlen(s1) - 1) == 0)
								{
									fprintf(file_b, "%s\n", buff);
									count++;
									break;
								}
						}
					else
						{
							if (strstr(word, s1))
								{
									fprintf(file_b, "%s\n", buff);
									count++;
									break;
								}
						}
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

	*r = count;

	return SUCCESS;

}


io_status task05 (const char *filename_a, const char *filename_b, const char *s, const char *t, int *r)
{

	int count = 0, i = 0, j = 0;
	int cnt_of_words = 0, cnt_of_words_s = 0;
	char buff[LEN];
	char word[LEN];
	char word_s[LEN];
	FILE *file_a;
	FILE *file_b;
  int words_index[LEN];
  int words_s_index[LEN];
  int line_printed = 0;

	if (!(file_a = fopen(filename_a, "r"))) return ERROR_OPEN;
	if (!(file_b = fopen(filename_b, "w"))) 
		{
			fclose(file_a);
			return ERROR_OPEN;
		}

	cnt_of_words_s = words_div(s, t, words_s_index);
	while (fgets(buff, LEN, file_a))
		{
			line_printed = 0;
			cnt_of_words = words_div(buff, t, words_index);
			buff[strlen(buff) - 1] = '\0';
			for (i = 0; i < cnt_of_words; i++)
				{
					if (line_printed == 1) break;
					for (j = 0; j < cnt_of_words_s; j++)
						{
							strncpy(word, buff + words_index[2 * i], words_index[2 * i + 1] - words_index[2 * i] + 1);
							word[words_index[2 * i + 1] - words_index[2 * i] + 1] = '\0';
							strncpy(word_s, s + words_s_index[2 * j], words_s_index[2 * j + 1] - words_s_index[2 * j] + 1);
							word_s[words_s_index[2 * j + 1] - words_s_index[2 * j] + 1] = '\0';
							if (strcmp(word, word_s) == 0)
								{
									fprintf(file_b, "%s\n", buff);
									count++;
									line_printed = 1;
									break;
								}
						}
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

	*r = count;

	return SUCCESS;
}

io_status task06 (const char *filename_a, const char *filename_b, const char *s, int *r)
{

	int count = 0, flag;
	char buff[LEN];
	char s1[LEN];
	char s2[LEN];
	io_status ret;
	FILE *file_a;
	FILE *file_b;
  size_t i;

	if (!(file_a = fopen(filename_a, "r"))) return ERROR_OPEN;
	if (!(file_b = fopen(filename_b, "w"))) 
		{
			fclose(file_a);
			return ERROR_OPEN;
		}

	ret = processing_s(s, s1, s2, '.', 2);

	if (ret != SUCCESS) 
		{
			fclose(file_a);
			fclose(file_b);
			return ret;
		}

	while (fgets(buff, LEN, file_a))
		{
			flag = 1;
			for (i = 0; buff[i] && s1[i]; i++)
				{
					if ((buff[i] != s1[i] && s2[i] != '1') || (strlen(buff) - 1 != strlen(s1))) 
						{
							flag = 0;
							break;
						}
				}
			if (flag)
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

	*r = count;

	return SUCCESS;

}

io_status task07 (const char *filename_a, const char *filename_b, const char *s, int *r)
{

	int count = 0, flag;
	char buff[LEN];
	char s1[LEN];
	char s2[LEN];
	io_status ret;
	FILE *file_a;
	FILE *file_b;
  size_t i = 0, j = 0, len_s;

	if (!(file_a = fopen(filename_a, "r"))) return ERROR_OPEN;
	if (!(file_b = fopen(filename_b, "w"))) 
		{
			fclose(file_a);
			return ERROR_OPEN;
		}

	ret = processing_s_7_9 (s, s1, s2, '?');
	len_s = strlen(s1);

	if (ret != SUCCESS) 
		{
			fclose(file_a);
			fclose(file_b);
			return ret;
		}

	while (fgets(buff, LEN, file_a))
		{
			flag = 1;
			i = 0;
			j = 0;
			while (buff[i] && s1[j])
				{
					if (s2[j] == '1')
						{
							if (s1[j] == buff[i])
								{
									i ++;
									j ++;
								}
							else 
								{
									j ++;
								}
						}
					else
						{
							if (s1[j] == buff[i])
								{
									i ++;
									j ++;
								}
							else
								{
									flag = 0;
									break;
								}
						}
				}

			if (i < strlen(buff) - 1) flag = 0;
			else if (j < len_s - 1)
				{
					while (j < len_s)
						{
							if (s2[j] == '0') flag = 0;
							j ++;
						}
				}

			if (flag)
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

	*r = count;

	return SUCCESS;

}


io_status task08 (const char *filename_a, const char *filename_b, const char *s, int *r)
{

	int count = 0, flag;
	char buff[LEN];
	char s1[LEN];
	char s2[LEN];
	io_status ret;
	FILE *file_a;
	FILE *file_b;
  size_t i = 0, j = 0;

	if (!(file_a = fopen(filename_a, "r"))) return ERROR_OPEN;
	if (!(file_b = fopen(filename_b, "w"))) 
		{
			fclose(file_a);
			return ERROR_OPEN;
		}

	ret = processing_s_7_9 (s, s1, s2, '+');


	if (ret != SUCCESS) 
		{
			fclose(file_a);
			fclose(file_b);
			return ret;
		}

	while (fgets(buff, LEN, file_a))
		{
			flag = 1;
			i = 0;
			j = 0;
			while (buff[i] && s1[j])
				{
					if (s2[j] == '1')
						{
							if (buff[i] == s1[j])
								{
									if (s1[j] == s1[j + 1])
										{
											j ++;
										}
									if (s1[j] != s1[j + 1])
										{
											while (buff[i] == buff[i + 1])
												{
													i ++;
												}
											i ++;
											j ++;
										}
								}
							else
								{
									flag = 0;
									break;
								}
						}
					else
						{
							if (buff[i] == s1[j])
								{
									i ++;
									j ++;
								}
							else
								{
									flag = 0;
									break;
								}
						}
				}
			if (i < strlen(buff) - 1) flag = 0;
			if (flag)
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

	*r = count;

	return SUCCESS;

}

io_status task09 (const char *filename_a, const char *filename_b, const char *s, int *r)
{

	int count = 0, flag;
	char buff[LEN];
	char s1[LEN];
	char s2[LEN];
	io_status ret;
	FILE *file_a;
	FILE *file_b;
  size_t i = 0, j = 0;

	if (!(file_a = fopen(filename_a, "r"))) return ERROR_OPEN;
	if (!(file_b = fopen(filename_b, "w"))) 
		{
			fclose(file_a);
			return ERROR_OPEN;
		}

	ret = processing_s_7_9 (s, s1, s2, '*');

	if (ret != SUCCESS) 
		{
			fclose(file_a);
			fclose(file_b);
			return ret;
		}

	while (fgets(buff, LEN, file_a))
		{
			flag = 1;
			i = 0;
			j = 0;
			while (buff[i] && s1[j])
		    {
		     if (s2[j] == '1')
		      {
		       if (buff[i] == s1[j])
		        {
		         if (s1[j] == s1[j + 1])j ++;
		         if (s1[j] != s1[j + 1])
		         	{
		          	while (buff[i] == buff[i + 1]) i ++;
		           	i ++;
		           	j ++;
		         	}
		        }
		       else j ++;
		      }
		     else
		      {
		       if (buff[i] == s1[j])
		        {
		         	i ++;
		         	j ++;
		        }
		       else
		        {
		         	flag = 0;
		         	break;
		        }
		      }
		    }
			if (s1[j] != '\0' || (buff[i] != '\0' && buff[i] != '\n')) flag = 0;
			if (flag)
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

	*r = count;

	return SUCCESS;

}

io_status task10 (const char *filename_a, const char *filename_b, const char *s, int *r)
{

	int count = 0, flag = 0;
	char buff[LEN];
	char s1[LEN];
	char s2[LEN];
	size_t i, j;
	io_status ret;
	FILE *file_a;
	FILE *file_b;

	if (!(file_a = fopen(filename_a, "r"))) return ERROR_OPEN;
	if (!(file_b = fopen(filename_b, "w"))) 
		{
			fclose(file_a);
			return ERROR_OPEN;
		}

	ret = processing_s_10 (s, s1, s2);

	if (ret != SUCCESS) 
		{
			fclose(file_a);
			fclose(file_b);
			return ret;
		}

	while (fgets(buff, LEN, file_a))
		{
			flag = 1;
			i = 0;
			j = 0;
			while (buff[i] && s1[j]) 
				{
          if (s2[j] == '1') 
	          {
	              if (!(buff[i] >= s1[j] && buff[i] <= s1[j + 1])) 
		              {
	                  flag = 0;
	                  break;
		              }
	              i++;
	              j += 2;
	          } 
          else 
	          {
              if (buff[i] == s1[j]) 
	              {
                  i++;
                  j++;
	              } 
              else 
	              {
                  flag = 0;
                  break;
	              }
          	}
        }
      if (s1[j] != '\0' || (buff[i] != '\0' && buff[i] != '\n')) flag = 0;
      if (flag) 
	      {
          fprintf(file_b, "%s", buff);
          count++;
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

	*r = count;

	return SUCCESS;

}


