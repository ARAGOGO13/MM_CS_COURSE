#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include "solve.h"

int lagrange_interpolation (double x0, double *x, double *y, int n, double *r)
{
	int i = 0, j = 0;
	double result = 0;
    for (i = 0; i < n; i++) 
	    {
	        double term = y[i];
	        for (j = 0; j < n; j++) 
		        {
		            if (j != i) 
			            {
                            if (fabs(x[j] - x0) < EPS)
                                {
                                    *r = y[j];
                                    return 0;
                                }
			            	if (fabs(x[i] - x[j]) < EPS) return -1;
			                term *= (x0 - x[j]) / (x[i] - x[j]);
			            }
		        }
	        result += term;
	    }
	*r = result;	
	return 0;
}

int newton_interpolation (double x0, double *x, double *y, int n, double *r)
{
    double result;
    for (int i = 0; i < n; i++)
	    {
                for (int j = n - 1; j > i; j--)
                        {
                            double denom = x[j] - x[j - i - 1];
                            double mx = fmax(fabs(x[j - i]), fabs(x[j]));
                            if (fabs(x[i] - x0) < EPS)
                                {
                                    *r = y[i];
                                    return 0;
                                }
                            if (j != j - i - 1 && fabs(x[j - i - 1] - x[j]) < EPS * mx) return -1;
                            y[j] = (y[j] - y[j - 1]) / denom;
                        }
	    }
    result = y[n - 1];
    for (int i = n - 2; i >= 0; i--)
        {
            result = result * (x0 - x[i]) + y[i];
        }

    *r = result;
    return 0;
}

int eitken_interpolation (double x0, double *x, double *y, int n, double *result) 
{
    if (!x || !y || !result || n <= 0) return -1;

    for (int k = 1; k < n; ++k) {
        for (int i = 0; i < n - k; ++i) 
            {

                double xi   = x[i];
                double xik  = x[i + k];

                double Li   = y[i];
                double Li1  = y[i + 1];

                double num  = (xi - x0) * Li1 - (xik - x0) * Li;
                double den  = xi - xik;

                if (fabs(xi - x0) < EPS)
                    {
                        *result = Li;
                        return 0;
                    }

                if (fabs(den) < EPS) return -1;

                y[i] = num / den;
            }
    }

    *result = y[0];
    return 0;
}

int lagrange_interpolation_d (double x0, double *x, double *y, double *d, int n, double *r)
{
	double result = 0.0;
	double L, Lprime, L2, h, g;

    for (int i = 0; i < n; i++) 
    {
        L = 1.0;   
        Lprime = 0.0;
        for (int j = 0; j < n; j++)
	        {
	            if (j == i) continue;
                if (fabs(x[j] - x0) < EPS)
                    {
                        *r = y[j];
                        return 0;
                    }
	            L *= (x0 - x[j]) / (x[i] - x[j]);
	        }

        for (int j = 0; j < n; j++) 
	        {
	            if (j == i) continue;
	            Lprime += 1.0 / (x[i] - x[j]);
	        }

        L2 = L * L;

        h = (1.0 - 2.0 * (x0 - x[i]) * Lprime) * L2;
        g = (x0 - x[i]) * L2;

        result += h * y[i] + g * d[i];
    }
    *r = result;
    return 0;
}

int taylor_sin (double x, double eps, double *r)
{
    int n = 1;
    double term, sum;

    if (eps <= 0)
        {
            return -2;
        }

    x = fmod(x, 2.0 * M_PI);

    term = x;
    sum = term;

    while (fabs(term) >= eps)
	    {
            n += 2;
            term = -term * x * x / ((n) * (n - 1));
	        sum += term;
	    }
    
    *r = sum;
    return 0;
}

int taylor_cos (double x, double eps, double *r)
{
    int n = 1;
    double term, sum;

    if (eps <= 0)
        {
            return -2;
        }

    x = fmod(x, 2.0 * M_PI);

    term = 1;
    sum = term;

    while (fabs(term) >= eps)
            {
                term *= -x * x / ((2 * n - 1) * (2 * n));
                sum += term;
                n++;
            }
    
    *r = sum;
    return 0;
}

int taylor_e (double x, double eps, double *result)
{
    double r = fmod(x, 1);
    double int_x = x - r;

    double int_pow = 1.0;
    double sum, term;
    int n = 1;

    if (eps <= 0)
        {
            return -2;
        }

    term = 1;
    sum = term;

    for (int i = 0; i < fabs(int_x); i++)
            {
                int_pow *= M_E;
            }
    if (int_x < 0)
        {
            int_pow = 1./int_pow;
        }

	do
        {
            term *= r / n;
            sum += term;
            n++;
        } while (fabs(term) >= eps);


	*result = sum * int_pow;
	return 0;
}

double taylor_ln(double x, double eps, double *r) 
{

    int b = 0, n = 1;
    double a = x;
    double t, t_sq, term, sum;

    if (eps <= 0)
        {
            return -2;
        }

    if (x <= 0) return -1;

    while (a >= 1.0) 
	    { 
	        a /= 2.0;
	        b++;
	    }
    while (a < 0.5) 
	    {
	        a *= 2.0;
	        b--;
	    }

    t = (a - 1.0) / (a + 1.0); 
    t_sq = t * t;
    term = t;
    sum = term;
    n = 1;

    do 
	    {
	        term *= t_sq * (2*n - 1) / (2*n + 1);
	        sum += term;
	        n++;
            } while (fabs(term) >= eps);

    sum *= 2.0;

    *r = sum + b * M_LN2;
    return 0;
}



