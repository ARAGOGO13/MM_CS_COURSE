#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "solve.h"
#include "array_io.h"

char *strcasestr_ (const char *string1, const char *string2) 
{
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

int task01 (char **a, int n, char *s)
{
	int i = 0, k = 0, j = 0;
	for (i = 0, j = 0; i < n; i++)
		{
			if (strcmp(a[i], s) >= 0)
				{
					if (i != j)
						{
							free(a[j]);
							a[j] = a[i];
							a[i] = 0;
						}
					j++;
				}
		}
	for (k = j; k < n; k ++)
		{
			if (a[k])
				{
					free(a[k]);
					a[k] = 0;
				}
		}

	return j;
}


int task02 (char **a, int n, char *s)
{
	int i = 0, k = 0, j = 0;
	for (i = 0, j = 0; i < n; i++)
		{
			if (strstr(s, a[i]) == 0)
				{
					if (i != j)
						{
							free(a[j]);
							a[j] = a[i];
							a[i] = 0;
						}
					j++;
				}
		}
	for (k = j; k < n; k ++)
		{
			if (a[k])
				{
					free(a[k]);
					a[k] = 0;
				}
		}

	return j;
}


int task03 (char **a, int n, char *s)
{
	int i = 0, k = 0, j = 0;
	size_t len_buff = 0;
	for (i = 0, j = 0; i < n; i++)
		{
			len_buff = strlen(a[i]);
            if (strcspn(a[i], s) != len_buff)
				{
					if (i != j)
						{
							free(a[j]);
							a[j] = a[i];
							a[i] = 0;
						}
					j++;
				}
		}
	for (k = j; k < n; k ++)
		{
			if (a[k])
				{
					free(a[k]);
					a[k] = 0;
				}
		}

	return j;
}


int task04 (char **a, int n, char *s)
{
        int i = 0, k = 0, j = 0;
        size_t len_s = strlen(s);
        size_t len_buff = 0;
        int flag = 0;
        for (i = 0, j = 0; i < n; i++)
            {
                len_buff = strlen(a[i]);
                if (len_buff >= len_s)
                    {
                      if (i != j)
                            {
                                free(a[j]);
                                a[j] = a[i];
                                a[i] = 0;
                            }
                      flag = 0;
                      j++;
                    }
                else
                    {
                        if (!flag)
                            {
                                if (i != j)
                                    {
                                        free(a[j]);
                                        a[j] = a[i];
                                        a[i] = 0;
                                    }
                                flag = 1;
                                j++;
                            }
                    }
            }

        for (k = j; k < n; k ++)
                {
                        if (a[k])
                                {
                                        free(a[k]);
                                        a[k] = 0;
                                }
                }


        return j;
}

int task05 (char **a, int n, char *s)
{
	int i = 0, k = 0, j = 0;
	size_t len_s = strlen(s);
	size_t len_buff = 0;
	int flag = 0;
	for (i = 0, j = 0; i < n; i++) 
            {
                len_buff = strlen(a[i]);
                if (strcmp(a[i] + len_buff - len_s, s) != 0)
                    {
                      if (i != j)
                            {
                                free(a[j]);
                                a[j] = a[i];
                                a[i] = 0;
                            }
                      flag = 0;
                      j++;
                    }
                else
                    {
                        if (!flag)
                            {
                                if (i != j)
                                    {
                                        free(a[j]);
                                        a[j] = a[i];
                                        a[i] = 0;
                                    }
                                flag = 1;
                                j++;
                            }
                    }
            }

	for (k = j; k < n; k ++)
		{
			if (a[k])
				{
					free(a[k]);
					a[k] = 0;
				}
		}


	return j;
}


int task06 (char **a, int n, char *s)
{
	int i = 0, k = 0, j = 0;
	size_t len_buff = 0;
	int flag = 0;
	for (i = 0, j = 0; i < n; i++) 
		{
		len_buff = strlen(a[i]);
		if (i == n - 1)
			{
				if (i != j)
					{
						free(a[j]);
						a[j] = a[i];
						a[i] = 0;
					}
				j++;
			}
      	else if (strcspn(a[i], s) != len_buff) 
	        {
				if (i != j)
					{
						free(a[j]);
						a[j] = a[i];
						a[i] = 0;
					}
				flag = 0;
				j++;
	        } 
	    else
		    {
		      	if (!flag)
		      		{
		      			if (i != j)
		      				{
			      				free(a[j]);
				              	a[j] = a[i];
				              	a[i] = 0;
		      				}
		      			j++;
		      		}
		      	else 
		      		{
		      			if (strcspn(a[i + 1], s) != strlen(a[i + 1]))
		      				{
		      					if (i != j)
				      				{
					      				free(a[j]);
						              	a[j] = a[i];
						              	a[i] = 0;
				      				}
				      			j++;
		      				}
		      		}
		      	flag = 1;
	        }
    }

	for (k = j; k < n; k ++)
		{
			if (a[k])
				{
					free(a[k]);
					a[k] = 0;
				}
		}


	return j;
}

int task07 (char **a, int n, char *s)
{
	int i = 0, k = 0, j = 0;
	int flag = 0;
	char *tmp = 0;
	for (i = 0, j = 0; i < n; i++)
		{
			tmp = a[i];
			if (a[i + 1] && !(flag || strcmp(a[i + 1], s) < 0))
				{
					if (i != j)
						{
							free(a[j]);
							a[j] = a[i];
							a[i] = 0;
						}
					j++;
				}
			if (!a[i + 1] && !flag) 
				{
					free(a[j]);
					a[j] = a[i];
					a[i] = 0;
					j++;
				}
			if (strcmp(tmp, s) < 0) flag = 1;
			else flag = 0;
		}
	for (k = j; k < n; k ++)
		{
			if (a[k])
				{
					free(a[k]);
					a[k] = 0;
				}
		}

	return j;
}

int task08 (char **a, int n, char *s)
{
	int i = 0, k = 0, j = 0;
	int flag = 0;
	char *tmp = 0;
	for (i = 0, j = 0; i < n; i++)
		{
			tmp = a[i];
			if (a[i + 1] && !(flag || strcasestr_(s, a[i + 1])))
				{
					if (i != j)
						{
							free(a[j]);
							a[j] = a[i];
							a[i] = 0;
						}
					j++;
				}
			if (!a[i + 1] && !flag) 
				{
					free(a[j]);
					a[j] = a[i];
					a[i] = 0;
					j++;
				}
			if (strcasestr_(s, tmp)) flag = 1;
			else flag = 0;
		}

	for (k = j; k < n; k ++)
		{
			if (a[k])
				{
					free(a[k]);
					a[k] = 0;
				}
		}

	return j;
}





