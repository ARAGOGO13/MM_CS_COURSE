#include "io_status.h"
#define LEN 1234

io_status read_array (char **a, int n, const char *name);
void free_array (char **a, int n);
void print_array (char **a, int n, int m);


int up_strcmp(const char *a, const char *b);
int down_strcmp(const char *a, const char *b);
int up_len(const char *a, const char *b);
int down_len(const char *a, const char *b);

int check (char **a, int n, int (*cmp) (const char*, const char*));

