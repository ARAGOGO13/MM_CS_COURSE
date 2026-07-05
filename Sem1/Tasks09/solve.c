#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include "solve.h"

size_t strlen_(const char *s) 
{
  size_t i = 0;
  for (i = 0; s[i]; i++);
  return i;
}

char *strcpy_(char *s1, const char *s2) 
{
  size_t i;
  char *tmp = s1;
  for (i = 0; (s1[i] = s2[i]); i++);
  return tmp;
}

char *strchr_(const char *s, int ch)
{
  size_t i;
  for (i = 0; s[i]; i++) 
    { 
      if (s[i] == (char) ch) return (char *)s + i;
    }
  if (s[i] == (char) ch) return (char *)s + i;
  return 0;
}  

char *strrchr_(const char *s, int ch)
{
  size_t i;
  size_t ind = 0;
  for (i = 0; s[i]; i++)
    {
      if (s[i] == (char) ch) ind = i + 1;
    }
  if (s[i] == (char) ch) ind = i + 1;
  if (ind == 0) return 0;
  return (char *)s + (ind - 1);
}


char* strcat_(char* s1, const char* s2) 
{
  char* original_s1 = s1;
  size_t i;
  s1 += strlen_(s1);
  for (i = 0; (s1[i] = s2[i]); i++);
    
  return original_s1;
} 

int strcmp_ (const char *s1, const char *s2)
{
  size_t i = 0;
  while (s1[i] && s2[i] && s1[i] == s2[i]) i++;
  return s1[i] - s2[i];
}

size_t strcspn_ (const char *s1, const char *s2)
{
  size_t i = 0;
  char alph[256] = {0};
  size_t cnt = 0;
  for (i = 0; s2[i]; i++)
    {
      alph[(int)(s2[i])] = 1;
    }

  for (i = 0; s1[i]; i++)
    {
      if (alph[(int)(s1[i])] == 0) cnt ++;
      else break;
    }
  return cnt;
}

size_t strspn_ (const char *s1, const char *s2)
{
  size_t i = 0;
  char alph[256] = {0};
  size_t cnt = 0;
  for (i = 0; s2[i]; i++)
    {
      alph[(int)(s2[i])] = 1;
    }

  for (i = 0; s1[i]; i++)
    {
      if (alph[(int)(s1[i])] == 1) cnt ++;
      else break;
    }
  return cnt;
}

char *strstr_ (const char *string1, const char *string2) {

    size_t i, j;
    size_t len1 = strlen(string1);
    size_t len2 = strlen(string2);

    if (len2 == 0) return (char *)string1;

    for (i = 0; i <= len1; i++) {
        if (string1[i] == string2[0]) {
            for (j = 1; j < len2; j++) {
                if (string1[i + j] != string2[j]) {
                    break;
                }
            }
            if (j == len2) {
                return (char *)&string1[i];
            }
        }
    }
    return (char *)&string1[len1];
}


char *strtok_r_ (char *str, const char* delim, char **saveptr) {
  size_t start, end;

  if (!str) {
    *saveptr = 0;
    return 0;
  }

  start = strspn(str, delim);
  str += start;

  if (strlen(str) == 0) 
    {
      *saveptr = 0;
      return 0;
    }

  end = strcspn(str, delim);

  if (str[end] != '\0') 
    {
      str[end] = '\0';
      *saveptr = str + end + 1;
    } 
  else 
    {
      *saveptr = 0;
    }

  return str;
}

  








