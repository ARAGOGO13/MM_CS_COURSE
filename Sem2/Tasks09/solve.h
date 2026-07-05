#include "array_io.h"

int lagrange_interpolation (double x0, double *x, double *y, int n, double *r);
int newton_interpolation (double x0, double *x, double *y, int n, double *r);
int eitken_interpolation (double x0, double *x, double *y, int n, double *r);
int lagrange_interpolation_d (double x0, double *x, double *y, double *d, int n, double *result);
int taylor_sin (double x, double eps, double *r);
int taylor_cos (double x, double eps, double *r);
int taylor_e (double x, double eps, double *r);
double taylor_ln(double x, double eps, double *r);





