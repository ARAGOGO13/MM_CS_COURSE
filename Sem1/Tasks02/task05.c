int printf (const char*, ...);
int sscanf (const char *, const char *, ...);
double sqrt (double);
int quadratic_equation (double, double, double, double*, double*);

int quadratic_equation (double a, double b, double c, double *x1, double *x2) 
{
	double discrim = b * b - 4 * a * c;
	if ((a >= 0 && a <= 0) && (b <= 0 && b >= 0) && (c <= 0 && c >= 0)) 
		{	
			*x1 = 0;
			*x2 = 0;
			return 0;
		}
	if (a <= 0 && a >= 0)	
	{
		*x1 = -c / b;
		*x2 = 0;
		return 1;
	}
	if (discrim < 0) 
		{
			*x1 = 0;
			*x2 = 0;
			return 0;
		}
	else if (discrim > 0) 
		{	
			if (a < 0) 
				{
					a *= -1;
					b *= -1;
					c *= -1;
				}
			if (b >= 0) 
				{
					*x1 = (-b - sqrt(discrim)) / (2 * a);
					*x2 = c / (a * ((-b - sqrt(discrim)) / (2 * a)));
					return 2;
				}
			else 
				{
					*x1 = (-b + sqrt(discrim)) / (2 * a);
					*x2 = c / (a * ((-b + sqrt(discrim)) / (2 * a)));
					return 2;
				}
		}
	*x1 = -b / 2 * a;
	*x2 = 0;
	return 1;
}

int main(int argc, char* argv[]) 
{
	double a, b, c, x1, x2;
	int res;
	if (argc != 4)
		{	
			printf("Can not read from %s \n", argv[0]);
			return 1;
		}
	if (sscanf(argv[1], "%lf", &a) != 1 || sscanf(argv[2], "%lf", &b) != 1 || sscanf(argv[3], "%lf", &c) != 1)
		{	
			printf("Can not read from %s \n", argv[1]);
			return 1;
		}
	res = quadratic_equation (a, b, c, &x1, &x2);
	printf ("%s : Task = %d Num = %d X1 = %e X2 = %e\n", argv[0], 5, res, x1, x2);
	return 0;
}