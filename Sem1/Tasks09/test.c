#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>
#include "solve.h"
#include "test.h"

double test_1 (size_t n, size_t (*p)(const char *), const char *s, size_t *result)
{
	size_t i;
	double t; 
	t = clock();
	for (i = 0; i < n; i++)
		{	
			*result = (*p) (s);
		}
	t = (clock() - t) / CLOCKS_PER_SEC;
	return t;
}


double test_2_5(size_t n, char* (*p)(char*, const char*), char* s1, const char* s2, char **result) {
  double t;
  size_t i;
  size_t len_s1 = strlen(s1);
  char *r = 0;
  t = clock();
  for (i = 0; i < n; ++i) 
    {
    	s1[len_s1] = 0;
      r = (*p) (s1, s2);
    }
  t = (clock() - t) / CLOCKS_PER_SEC;
  *result = r;

  return t;
}

double test_3_4 (size_t n, char* (*p) (const char*, int), const char *s, int ch, char **result)
{
	size_t i;
	double t = clock();
	for (i = 0; i < n; i++)
		{
			*result = (*p) (s, ch);
		}
	t = (clock() - t) / CLOCKS_PER_SEC;
	return t;
}


double test_6 (size_t n, int (*p) (const char*, const char *), const char *s1, const char *s2, int *result)
{
	size_t i;
	double t = clock();
	for (i = 0; i < n; i++)
		{
			*result = (*p) (s1, s2);
		}
	t = (clock() - t) / CLOCKS_PER_SEC;
	return t;
}

double test_7_8 (size_t n, size_t (*p) (const char*, const char *), const char *s1, const char *s2, size_t *result)
{
	size_t i;
	double t = clock();
	for (i = 0; i < n; i++)
		{
			*result = (*p) (s1, s2);
		}
	t = (clock() - t) / CLOCKS_PER_SEC;
	return t;
}

double test_9 (size_t n, char* (*p) (const char *, const char *), char *s1, const char *s2, char **result)
{
	size_t i;
	double t = clock();
	for (i = 0; i < n; i++)
		{
			*result = (*p) (s1, s2);
		}
	t = (clock() - t) / CLOCKS_PER_SEC;
	return t;
}

double test_10 (size_t n, char* (*p) (char*, const char *, char**), char *str, const char *delim, char **saveptr, char *original_str, char **res)
{
	size_t i;
	double t;
	char *r = 0;
	t = clock();

	for (i = 0; i < n; i++)
		{
			strcpy_(str, original_str);
			r = (*p) (str, delim, saveptr);
		}
	t = (clock() - t) / CLOCKS_PER_SEC;
	*res = r;
	return t;
}



