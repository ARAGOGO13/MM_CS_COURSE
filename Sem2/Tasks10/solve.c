#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include "solve.h"

static int count = 0;

double f0(double x) { (void)x; count ++; return 1.0; }
double f1(double x) { count ++; return x - 1e100; }
double f2(double x) { count ++; return 4.0 - x*x; }
double f3(double x) { count ++; return x*x*x + 3.0*x*x + 16.0; }
double f4(double x) { count ++; return 3.0 - 2.0*(x*x) - (x*x*x*x); }
double f5(double x) { count ++; return sqrt(fabs(x) + 1.0) - 2.0; }
double f6(double x) { count ++; return sqrt( sqrt(fabs(x) + 1.0) + 1.0 ) - 2.0; }

double d0(double x) { (void)x; return 0.0; }
double d1(double x) { (void)x; return 1.0; }
double d2(double x) { return -2*x; }
double d3(double x) { return 3*x*x + 6.0*x; }
double d4(double x) { return 4.0*x - 4*(x*x*x); }
double d5(double x) { return x / (2 * fabs(x) * sqrt(fabs(x) + 1)); }
double d6(double x) { return x / (4 * fabs(x) * sqrt(fabs(x) + 1) * sqrt(sqrt(fabs(x) + 1) + 1)); }

int get_count (void) { return count; }


int bisection_method (double (*f)(double), double a, double b, double eps, int M, double *x)
{
    double fa = f(a);
    double fb = f(b);
    double c = 0, fc;
    int it = 0;

    if (fabs(fa) < eps) 
        {
            *x = a;
            return it;
        }
    if (fabs(fb) < eps) 
        {
            *x = b;
            return it;
        }

    if (fa * fb > 0) 
        {
            return -1;
        }

    for (it = 1; it <= M; it ++) 
        {
            c  = (a + b) / 2.0;
            fc = f(c);

            if (fabs(fc) < eps) 
                {
                    *x = c;
                    return it;
                }

            if (fa * fc <= 0) 
                {
                    b = c;
                    fb = fc;
                } 
            else 
                {
                    a = c;
                    fa = fc;
                }
        }

    *x = c;

    if (it >= M) 
        {
            return -1;
        }

    return it;
}

int newton_method (double (*f)(double), double (*d)(double), double x0, double eps, int M, double *x)
{
    int it;
    double fx  = f(x0);
    double dfx = d(x0);

    if (fabs(fx) < eps) 
        {
            *x = x0;
            return 0;
        }

    if (dfx <= 0.0 && dfx >= 0) 
        {
            return -1;
        }

    for (it = 1; it <= M; it ++) 
        {
            x0 = x0 - fx / dfx;

            fx  = f(x0);
            dfx = d(x0);

            if (fabs(fx) < eps) 
                {
                    *x = x0;
                    return it;
                }

            if (dfx <= 0.0 && dfx >= 0) 
                {
                    return -1;
                }
        }

    if (it >= M) 
        {
            return -1;
        }

    return it;
}

int chord_method(double (*f)(double), double a, double b, double eps, int M, double *x)
{
    int it = 0;
    double fa = f(a);
    double fb = f(b);
    double xk = a;

    double fxk;

    if (fabs(fa) < eps) 
        {
            *x = a;
            return it;
        }
    if (fabs(fb) < eps) 
        {
            *x = b;
            return it;
        }

    if (fa * fb >= 0.0) 
        {
            return -1;
        }

    for (it = 1; it <= M; it ++) 
        {
            xk = (a * fb - b * fa) / (fb - fa);
            fxk = f(xk);

            if (fabs(fxk) < eps) 
                {
                    *x = xk;
                    return it;
                }

            if (fa * fxk < 0.0) 
                {
                    b = xk;
                    fb = fxk;
                } 
            else 
                {
                    a = xk;
                    fa = fxk;
                }
        }

    if (it >= M) 
        {
            return -1;
        }
    return it;
}

int secants_method(double (*f)(double), double a, double b, double eps, int M, double *x)
{
    int it = 0;
    double fa = f(a);
    double fb = f(b);
    double xk = a;

    double fxk;

    if (fabs(fa) < eps) 
        {
            *x = a;
            return it;
        }
    if (fabs(fb) < eps) 
        {
            *x = b;
            return it;
        }

    if (fa * fb >= 0.0) 
        {
            return -1;
        }

    for (it = 1; it <= M; it ++) 
    {
        xk = b - fb * (b - a) / (fb - fa);
        fxk = f(xk);

        if (fabs(fxk) < eps) 
            {
                *x = xk;
                return it;
            }

        if (fa * fxk < 0.0) 
            {
                b = xk;
                fb = fxk;
            } 
        else 
            {
                a = xk;
                fa = fxk;
            }
    }

    if (it >= M) 
        {
            return -1;
        }

    return it;
}

int quadratic_interpolation_method(double (*f)(double), double a, double b, double eps, int M, double *x)
{
    int it = 0;
    double x0 = a, x1 = 0.5 * (a + b), x2 = b;
    double f0 = f(x0), f1 = f(x1), f2 = f(x2);
    double x3, f3;

    if (fabs(f0) < eps) { *x = x0; return it; }
    if (fabs(f1) < eps) { *x = x1; return it; }
    if (fabs(f2) < eps) { *x = x2; return it; }

    for (it = 1; it <= M; it++)
    {
        double denom0 = (f0 - f1) * (f0 - f2);
        double denom1 = (f1 - f0) * (f1 - f2);
        double denom2 = (f2 - f0) * (f2 - f1);

        if ((denom0 <= 0 && denom0 >= 0) || (denom1 <= 0 && denom1 >= 0) || (denom2 <= 0 && denom2 >= 0))
            return -1;

        x3 = x0 * (f1 * f2) / denom0
           + x1 * (f0 * f2) / denom1
           + x2 * (f0 * f1) / denom2;
        f3 = f(x3);

        if (fabs(f3) < eps)
            {
                *x = x3;
                return it;
            }

        if (fabs(f3) < fabs(f0)) { x0 = x3;  f0 = f3; }
        else if (fabs(f3) < fabs(f1)) { x1 = x3;  f1 = f3; }
        else if (fabs(f3) < fabs(f2)) { x2 = x3;  f2 = f3; }
        else return -1; 
    }

    if (it >= M) 
        {
            return -1;
        }

    return it;
}

int reverse_interpolation_method(double (*f)(double), int m, double *d, double a, double b, double eps, int M, double *x)
{
    int it, i, j, n, mx_ind;
    double x0, fx0, mx, el, xi, temp;
    n = m + 1;
    
    for (i = 0; i < n; i++) 
        {
            xi = a + (i * (b - a)) / m;
            d[i] = f(xi);
            d[n + i] = xi;
            d[2 * n + i] = xi;
        }

    for (i = 0; i < n; i++) 
        {
            for (j = i + 1; j < n; j++) 
                {
                    if ((d[i] <= d[j] && d[j] <= d[i]) || (d[j] <= d[i] && d[i] <= d[j])) return -1;
                }
        }

    for (it = 1; it <= M; it ++) 
        {
            for (i = 0; i < n; i ++) 
                {
                    for (j = n - 1; j > i; j --) 
                        {
                            temp = (d[j] - d[j - 1 - i]);
                            if (temp <= 0.0 && temp >= 0.0) return -1;
                            d[n + j] = (d[n + j] - d[n + j - 1]) / temp;
                        }
                }

            x0 = d[2 * n - 1];
            for (i = n - 2; i >= 0; i --) 
                {
                    x0 = x0 * (-d[i]) + d[n + i];
                }

            fx0 = f(x0);
            if (fabs(fx0) < eps) 
                {
                    *x = x0;
                    return it;
                }

            mx_ind = 0; mx = 0.0;
            for (i = n - 1; i >= 0; i --) 
                {
                    el = fabs(d[i]);
                    if (el > mx) { mx = el; mx_ind = i; }
                }

            for (i = 0; i < n; i ++) 
                {
                    if ((fx0 <= d[i] && d[i] <= fx0) || (d[i] <= fx0 && fx0 <= d[i])) return -1;
                    d[n + i] = d[2 * n + i];
                }

            d[mx_ind] = fx0;
            d[n + mx_ind] = x0;
            d[2 * n + mx_ind] = x0;
        }
    
    return -1;
}


int iterations_method(double (*f)(double), double x0, double eps, int M, double *x)
{
    double x_old = x0;
    double x_new;
    int it;

    x_new = f(x_old);
    if (fabs(x_new - x_old) < eps) 
        {
            *x = x_new;
            return 0;
        }

    for (it = 1; it <= M; it++) 
        {
            x_new = f(x_old);

            if (fabs(x_new - x_old) < eps) 
                {
                    *x = x_new;
                    return it;
                }

            x_old = x_new;
        }

    if (it >= M) 
        {
            return -1;
        }
    return it;
}


int linear_search_max(double (*f)(double), double a, double b, double eps, int M, double *x)
{
    double h = (b - a) / 10.0;
    double x_curr = a;
    double f_curr = f(x_curr);
    int it = 0;
    double f_next;

    for (it = 1; it <= M; it++) 
        {
            double x_next = x_curr + h;

            if (x_next < a) 
                {
                    *x = a;
                    return it;
                }
            if (x_next > b) 
                {
                    *x = b;
                    return it;
                }

            f_next = f(x_next);

            if (f_next > f_curr) 
                {
                    x_curr = x_next;
                    f_curr = f_next;
                } 
            else 
                {
                    h = -h / 10.0;
                }

            if (fabs(h) < eps) 
                {
                    *x = x_curr;
                    return it;
                }
        }

    if (it >= M) 
        {
            return -1;
        }
    return it;
}

int golden_section_max (double (*f)(double), double a, double b, double eps, int M, double *x)
{
    double phi = 2.0 - (sqrt(5.0) + 1.0) * 0.5;
    double psi = (sqrt(5) - 1) * 0.5;

    double x1 = a + phi * (b - a);
    double x2 = a + psi * (b - a);
    double f1 = f(x1);
    double f2 = f(x2);
    int it;

    for (it = 1; it <= M; it++)
        {
            if (f1 >= f2)
                {
                    b = x2;
                    x2 = x1;
                    f2 = f1;
                    x1 = a + phi * (b - a);
                    f1 = f(x1);
                }
            else
                {
                    a = x1;
                    x1 = x2;
                    f1 = f2;
                    x2 = a + psi * (b - a);
                    f2 = f(x2);
                }

            if (fabs(b - a) < eps)
                {
                    *x = (x1 + x2) * 0.5;
                    return it;
                }
        }
    x1 = a + phi * (b - a);
    x2 = a + psi * (b - a);
    if (it >= M && ((a <= x1 && a >= x1) || (b <= x2 && b >= x2))) 
        {
            *x = (x1 + x2) * 0.5;
            return it;
        }
    return -1;
}

