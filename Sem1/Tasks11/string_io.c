#include <stdio.h>
#include "string_io.h"

char *strncpy_(char *s1, const char *s2, size_t n) 
{
  size_t i;
  char *tmp = s1;
  for (i = 0; ((s1[i] = s2[i]) && i < n); i++);
  return tmp;
}

int strncmp_ (const char *s1, const char *s2, size_t n)
{
  size_t i = 0;
  while (i < n && s1[i] && s2[i] && s1[i] == s2[i]) i++;
  if (i == n) return 0;
  return s1[i] - s2[i];
}

char* strncat_(char* s1, const char* s2, size_t n) 
{
  char* original_s1 = s1;
  size_t i;
  s1 += strlen(s1);
  for (i = 0; ((s1[i] = s2[i]) && (i < n)); i++);
  s1[i] = 0;
  return original_s1;
} 

char *strcasestr_ (const char *string1, const char *string2) {

    size_t i, j;
    size_t len1 = strlen(string1);
    size_t len2 = strlen(string2);

    if (len2 == 0) return (char *)string1;

    for (i = 0; i <= len1; i++) 
    	{
        for (j = 0; j < len2; j++) 
          {
            unsigned char char1 = string1[i + j];
            unsigned char char2 = string2[j];

            if ( !((char1 >= 'A' && char1 <= 'Z' && char2 >= 'a' && char2 <= 'z' && (int)char1 + 32 == (int)char2) ||
             (char1 >= 'a' && char1 <= 'z' && char2 >= 'A' && char2 <= 'Z' && (int)char1 - 32 == (int)char2) ||
             (char1 == char2))) 
            	{
            		break;
            	}

          }
        if (j == len2) 
          {
            return (char *)&string1[i];
          }
    	}
    return 0;
}
