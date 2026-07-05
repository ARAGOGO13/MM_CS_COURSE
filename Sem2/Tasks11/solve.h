int get_count (void);
double f0(double x);
double f1(double x);
double f2(double x);
double f3(double x);
double f4(double x);
double f5(double x);
double f6(double x);

int derivative01 (double (*f)(double), double x, double h, double *dx);
int derivative02 (double (*f)(double), double x, double h, double *dx);
int second_derivative (double (*f)(double), double x, double h, double *dx);
int integral_trapezoid (double (*f)(double), double a, double b, int n, double *res);
int integral_simpson (double (*f)(double), double a, double b, int n, double *res);
int integral_trapezoid_weight (double (*f)(double), double a, double b, int n, double *res);
int integral_simpson_weight (double (*f)(double), double a, double b, int n, double *res);
int integral_trapezoid_step (double (*f)(double), double a, double b, double eps, double *res);
int integral_simpson_step(double (*f)(double), double a, double b, double eps, double *res);
int integral_trapezoid_inf (double (*f)(double), double a, double eps, double *res);
int integral_simpson_inf (double (*f)(double), double a, double eps, double *res);
int curve_length (double (*x)(double), double (*y)(double), double a, double b, double eps, double *r);




double pow32(double x);
double sgn(double x);
double hyp(double x, double y);









