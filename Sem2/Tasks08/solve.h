#include "array_io.h"

int is_symmetric (double *a, int n);
double r_solver (double *a, double *a_inv, int n, double *r1, double *r2);
int cholesky_inverse(int n, double* A, double* R, double* D);
double matrix_norm (double *A, int n);

int invert_upper(int n, double *U, double *U_inv);
void compute_inverse_from_Uinv(int n, double *U_inv, double *A_inv);

