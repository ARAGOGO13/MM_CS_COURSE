int printf (const char*, ...);
int sscanf (const char *, const char *, ...);
void max_min_f (double, double, double, double*, double*);

void max_min_f (double a, double b, double c, double *x1, double *x2) 
{
	*x1 = a;
	*x2 = a;
	if (b > *x1) *x1 = b;
	if (c > *x1) *x1 = c;
	if (b < *x2) *x2 = b;
	if (c < *x2) *x2 = c;
}

int main(int argc, char* argv[]) 
{
	double a, b, c, x1, x2;
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
	max_min_f (a, b, c, &x1, &x2);
	printf ("%s : Task = %d X1 = %e X2 = %e\n", argv[0], 4, x1, x2);
	return 0;
}