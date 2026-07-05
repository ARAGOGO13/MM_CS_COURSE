#include "io_status.h"

io_status read_matrix (double *a, int n, int m, const char* name);
io_status read_matrix1 (double *a, int n, int m, const char* name);

void print_matrix (const double *a, int n, int m, int p);
void init_matrix (double *a, int n, int m, int k);
double f (int k, int n, int m, int i, int j);
