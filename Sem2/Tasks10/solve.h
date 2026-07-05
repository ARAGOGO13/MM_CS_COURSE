int get_count (void);
double f0(double x);
double f1(double x);
double f2(double x);
double f3(double x);
double f4(double x);
double f5(double x);
double f6(double x);
double d0(double x);
double d1(double x);
double d2(double x);
double d3(double x);
double d4(double x);
double d5(double x);
double d6(double x);
int bisection_method (double (*f)(double), double a, double b, double eps, int M, double *x);
int newton_method (double (*f)(double), double (*d)(double), double x0, double eps, int M, double *x);
int chord_method(double (*f)(double), double a, double b, double eps, int M, double *x);
int secants_method (double (*f)(double), double a, double b, double eps, int M, double *x);
int quadratic_interpolation_method (double (*f)(double), double a, double b, double eps, int M, double *x);
int reverse_interpolation_method (double (*f)(double), int m, double *d, double a, double b, double eps, int M, double *x);
int iterations_method(double (*f)(double), double x0, double eps, int M, double *x);
int linear_search_max(double (*f)(double), double a, double b, double eps, int M, double *x);
int golden_section_max(double (*f)(double), double a, double b, double eps, int M, double *x);
int quadratic_interpolation_max(double (*f)(double), double a, double b, double eps, int M, double *x);














