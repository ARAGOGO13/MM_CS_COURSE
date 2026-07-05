#define ERROR_OPEN (-1)
#define ERROR_READ (-2)
#define SUCCESS (0)

double test_1 (size_t n, size_t (*p) (const char *), const char *s, size_t *result);
double test_2_5 (size_t n, char* (*p) (char *, const char *), char *s1, const char *s2, char **result);
double test_3_4 (size_t n, char* (*p) (const char*, int), const char *s, int ch, char **result);
double test_6 (size_t n, int (*p) (const char *, const char *), const char *s1, const char *s2, int *result);
double test_7_8 (size_t n, size_t (*p) (const char*, const char *), const char *s1, const char *s2, size_t *result);
double test_9 (size_t n, char* (*p) (const char*, const char *), char *s1, const char *s2, char **result);
double test_10 (size_t n, char * (*p) (char*, const char *, char**), char *str, const char *delim, char **saveptr, char *original_str, char **res);

