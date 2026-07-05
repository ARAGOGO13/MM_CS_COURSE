int printf (const char*, ...);
int sscanf (const char *, const char *, ...);
double num_sequance1 (int);

double num_sequance1 (int n) 
{
	double previous = 1;
	double x = 1;
	int i;
	if (n >= 0) 
		{
			double t = 2;
			for (i = 1; i <= n; i++) 
				{
					x = 3 * previous + 4 * t;
					t *= 2;
					previous = x;
				}
		}
	if (n < 0)
		{	
			double t = 1;
			for (i = 0; i > n; i--) 
				{	
					x = (previous - 4 * t) / 3;
					t /= 2;
					previous = x;
				}
		}
	return x;
}

int main(int argc, char* argv[]) 
{
	int n;
	double res;
	if (argc != 2)
		{	
			printf("Can not read from %s \n", argv[0]);
			return 1;
		}
	if ((sscanf(argv[1], "%d", &n)) != 1)
		{	
			printf("Can not read from %s \n", argv[1]);
			return 1;
		}
	res = num_sequance1 (n);
	printf ("%s : Task = %d Result = %e\n", argv[0], 6, res);
	return 0;
}