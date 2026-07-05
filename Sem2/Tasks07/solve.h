#include "array_io.h"

void matrix_vector_multiplication (double *a, double *vec_b, int m, double *vec_c, int n);
double scalar_product (double *a, double *b, double n);
void compute_vector_b (double *a, double *b, int n);
void vector_subtract(double *a, double *b, double *result, int n);
void solve_lower_triangular(double *a, double *rhs, double *sol, int n);
void solve_upper_triangular(double *a, double *rhs, double *sol, int n);
void solve_symmetric_system(double *a, double *b, double *w, int n);


double r1task01 (double *a, double *x0, double *x, int n, int m);
double r2task01 (double *a, double *x, int n, double r1);
task_status task02 (double *a, double *x0, double *x, double *b, int n, int m, double t);
task_status task03 (double *a, double *x0, double *x, double *b, double *r, int n, int m);
task_status task04 (double *a, double *x0, double *x, double *b, double *r, int n, int m);
task_status task05 (double *a, double *x0, double *x, double *b, double *r, int n, int m);
task_status task06 (double *a, double *x0, double *x, double *b, double *r, int n, int m);
task_status task07 (double *a, double *x0, double *x, double *b, double *r, int n, int m, double t);
task_status task08 (double *a, double *x0, double *x, double *b, double *r, double *w, int n, int m, double t);
task_status task09 (double *a, double *x0, double *x, double *b, double *r, double *w, int n, int m, double t);
task_status task10 (double *a, double *x0, double *x, double *b, double *r, double *w, int n, int m, double t);


double r1_solver (double *a, double *x, double *b, int n);
double r2_solver (double *x, int n);


