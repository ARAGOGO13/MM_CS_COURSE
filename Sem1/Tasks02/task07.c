int printf (const char*, ...);
int sscanf (const char *, const char *, ...);
double num_sequance2 (int);

double num_sequance2 (int n) 
{
	double previous_2 = 2;
	double previous_1 = 9;
	double x = 2;
	int i;
	if (n == 1) return 9; 
	else if (n >= 2) 
		{
			double t = 4;
			for (i = 2; i <= n; i++) 
				{
					x = 5 * previous_1 - 6 * previous_2 + t;
					t *= 4;
					previous_2 = previous_1;
					previous_1 = x;
				}
		}
	else if (n < 0)
		{	
			double t = 1;
			for (i = 0; i > n; i--) 
				{	
					x = (5 * previous_2 - previous_1 + t) / 6;
					t /= 4;
					previous_1 = previous_2;
					previous_2 = x;
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
	res = num_sequance2 (n);
	printf ("%s : Task = %d Result = %e\n", argv[0], 7, res);
	return 0;
}