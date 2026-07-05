int printf (const char*, ...);
int sscanf (const char *, const char *, ...);
double sum_sequance2 (double, int);

double sum_sequance2 (double x, int n) 
{
	int k;
	double degree = 1;
	double summ = 0;
	int one_k = 1;
	for (k = 1; k <= n; k ++) 
		{
			degree *= x;
			one_k *= -1;
			summ += one_k * degree / k;
		}
		return summ;
}

int main(int argc, char* argv[]) 
{
	double x, res;
	int n;
	if (argc != 3)
		{	
			printf("Can not read from %s \n", argv[0]);
			return 1;
		}
	if (sscanf(argv[1], "%lf", &x) != 1 || sscanf(argv[2], "%d", &n) != 1)
		{	
			printf("Can not read from %s \n", argv[1]);
			return 1;
		}
	res = sum_sequance2 (x, n);
	printf ("%s : Task = %d Result = %e\n", argv[0], 9, res);
	return 0;
}