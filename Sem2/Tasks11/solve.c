#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include "solve.h"

static int count = 0;

int get_count (void) { return count; }

double f0(double x) { (void)x; count ++; return 1.0; }
double f1(double x) { count ++; return x + 1; }
double f2(double x) { count ++; return 1 + x + x*x; }
double f3(double x) { count ++; return 1 + x + x*x + x*x*x; }
double f4(double x) { count ++; return 1 + x + x*x + x*x*x + x*x*x*x; }
double f5(double x) { count ++; return 1 / exp(x); }
double f6(double x) { count ++; return 1 / (1 + 25*x*x); }

int derivative01 (double (*f)(double), double x, double h, double *dx)
{
    if (h <= 0 && h >= 0) return -1;
    *dx = (f(x + h) - f(x)) / h;
    return 0;
}

int derivative02 (double (*f)(double), double x, double h, double *dx)
{
    if (h <= 0 && h >= 0) return -1;
    *dx = (f(x + h) - f(x - h)) / (2 * h);
    return 0;
}

int second_derivative (double (*f)(double), double x, double h, double *dx)
{
    if (h <= 0 && h >= 0) return -1;
    *dx = (f(x + h) - 2 * f(x) + f(x - h)) / (h * h);
    return 0;
}

int integral_trapezoid (double (*f)(double), double a, double b, int n, double *res)
{
    int i = 0;
    double sum = 0;
    double h = (b - a) / n;
    if (h <= 0 && h >= 0) return -1;

    sum = (f(a) + f(b)) / 2;

    for (i = 1; i < n; i ++)
        {
            sum += f(a + i * h);
        }
    *res = sum * h;
    return 0;
}

int integral_simpson (double (*f)(double), double a, double b, int n, double *res)
{
    int i = 0;
    double sum = 0;
    double h = (b - a) / (2 * n);
    if (h <= 0 && h >= 0) return -1;

    sum = (f(a) + f(b)) / 2;

    for (i = 1; i < 2 * n; i ++)
        {
            sum += ((i % 2 == 1) ? 2 * f(a + i * h) : f(a + i * h));
        }
    *res = 2. / 3 * h * sum;
    return 0;
}

double pow32(double x) { return fabs(x) * sqrt(fabs(x)); }
double sgn(double x) { return (x > 0) ? 1 : ((x < 0) ? -1 : 0); }

int integral_trapezoid_weight (double (*f)(double), double a, double b, int n, double *res)
 {
    double h = (b - a) / n;
    double sum = 0.0;
    double x0 = a, f0 = f(x0);
    double x1 = a + h, x2 = 0;

    if (n <= 0) return -1;

    sum += f0 * (
        (2./3) * pow32(x1) + 
        (1./3) * pow32(a) - 
        x1 * sqrt(fabs(a)) * sgn(a)
    );

    for (int i = 1; i < n; i++) 
        {
            x2 = a + (i + 1) * h;
            
            sum += f(x1) * (
                (2./3) * pow32(x0) + 
                (2./3) * pow32(x1) + 
                (2./3) * pow32(x2) - 
                2 * pow32(x1));
            x0 = x1;
            x1 = x2;
        }

    x1 = b - h;
    sum += f(b) * (
        (2./3) * pow32(x1) + 
        (1./3) * pow32(b) - 
        x1 * sqrt(fabs(b)) * sgn(b)
    );

    *res = 2 * sum / h;

    return 0;
}

int integral_trapezoid_step (double (*f)(double), double a, double b, double eps, double *res)
{
    int n = 1;
    int i = 0;
    double I_n = 0;
    double I_2n = 0;
    double sum = 0;
    double x = 0;
    double h = (b - a) / 2;
    int it = 0;
    int MAX_ITERS = 29;

    integral_trapezoid(f, a, b, n, &I_n);

    if (a - b <= 0 && a - b >= 0)
        {
            *res = 0;
            return 0;
        }   

    while (it < MAX_ITERS) 
        {
            int old_n = n;
            it ++;
            n *= 2;
            h = (b - a) / n;
            sum = 0.0;
            for (i = 0; i < old_n; ++i) 
                {
                    x = a + (2 * i + 1) * h;
                    sum += h * f(x);
                }
            I_2n = 0.5 * I_n + sum;
            if (fabs(I_2n - I_n) < eps) 
                {
                    *res = I_2n;
                    return it;
                }
            I_n = I_2n;
        }

    if (it < MAX_ITERS)
        {
            *res = I_2n;
            return it;
        }

    *res = 0;
    return -1;
}


int integral_simpson_step(double (*f)(double), double a, double b, double eps, double *res)
{
    int n = 2;
    int i = 0;
    int it = 1;
    int MAX_ITERS = 29;
    double h = 0.5 * (b - a);
    double I1, I2, I_n, I1_2n, I2_2n, I_2n;
    double sum;

    if (a - b <= 0 && a - b >= 0)
        {
            *res = 0;
            return 0;
        }

    I1  = h * (f(a) + f(b)) / 3.;
    I2  = 4 * h * f(a + h) / 3.;
    I_n = I1 + I2;

    I1_2n = 0.5 * (I1 + 0.5 * I2);
    I2_2n = 2 * h * (f(a + 0.5 * h) + f(a + 1.5 * h)) / 3.;
    I_2n  = I1_2n + I2_2n;

    while (it < MAX_ITERS && fabs(I_2n - I_n) >= eps) 
        {
            I1 = I1_2n;
            I2 = I2_2n;
            I_n = I_2n;
            h *= 0.5;
            n *= 2;

            sum = 0;
            for (i = 0; i < n; ++i) 
                {
                    sum += f(a + 0.5 * (2 * i + 1) * h);
                }

            I1_2n = 0.5 * (I1 + 0.5 * I2);
            I2_2n = 2 * h * sum / 3.;
            I_2n  = I1_2n + I2_2n;

            it ++;
        }

    if (it < MAX_ITERS) 
        {
            *res = I_2n;
            return it;
        } 
    else 
        {
            *res = 0.0;
            return -1;
        }
}

int integral_trapezoid_inf (double (*f)(double), double a, double eps, double *res)
{
    int MAX_ITERS = 30;
    double sum = 0;
    double h = 1;
    double b = a + h;
    int it = 1;
    double I;
    int n = integral_trapezoid_step(f, a, b, eps, &I);

    while (it < MAX_ITERS && n >= 0 && fabs(I) >= eps) 
        {
            sum += I;
            a = b;
            h *= 2;
            b = a + h;
            n = integral_trapezoid_step(f, a, b, eps, &I);
            it ++;
        }

    if (it < MAX_ITERS && n >= 0)
        {
            *res = sum;
            return b;
        }

    *res = 0.0;
    return -1;
}

int integral_simpson_inf (double (*f)(double), double a, double eps, double *res)
{
    int MAX_ITERS = 30;
    double sum = 0;
    double h = 1;
    double b = a + h;
    int it = 1;
    double I;
    int n = integral_simpson_step(f, a, b, eps, &I);

    while (it < MAX_ITERS && n >= 0 && fabs(I) >= eps) 
        {
            sum += I;
            a = b;
            h *= 2;
            b = a + h;
            n = integral_simpson_step(f, a, b, eps, &I);
            it ++;
        }

    if (it < MAX_ITERS && n >= 0)
        {
            *res = sum;
            return b;
        }

    *res = 0.0;
    return -1;
}

double hyp(double x, double y) {return sqrt(x*x + y*y); }

int curve_length (double (*x)(double), double (*y)(double), double a, double b, double eps, double *res)
{
    const int MAX_ITERS = 30;
    int n = 1;
    double h = (b - a) / n;

    double t0 = a, t1 = b;
    double x0 = x(t0), y0 = y(t0);
    double x1 = x(t1), y1 = y(t1);
    double S_prev = hyp(x1 - x0, y1 - y0);
    double sum, t_prev, x_prev, y_prev;

    for (int it = 1; it <= MAX_ITERS; ++it) 
        {
            n *= 2;
            h = (b - a) / n;
            sum = 0.0;
            t_prev = a;
            x_prev = x(t_prev);
            y_prev = y(t_prev);

            for (int i = 1; i <= n; ++i) 
                {
                    double t_curr = a + i * h;
                    double x_curr = x(t_curr), y_curr = y(t_curr);
                    sum += hyp(x_curr - x_prev, y_curr - y_prev);
                    x_prev = x_curr;
                    y_prev = y_curr;
                }

            if (fabs(sum - S_prev) < eps) 
                {
                    *res = sum;
                    return n;
                }

            S_prev = sum;
        }

    *res = 0;
    return -1;
}
