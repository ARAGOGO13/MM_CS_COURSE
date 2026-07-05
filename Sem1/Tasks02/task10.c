int printf (const char*, ...);
int sscanf (const char *, const char *, ...);
double sum_of_prime (int);
int is_prime (int);

int is_prime (int n) 
{
	int i;
	for (i = 2; i * i <= n; i ++) 
		{
			if (n % i == 0) return 0;
		}
	return 1;
}

double sum_of_prime (int n) 
{
	double summ = 0;
	int i;
	int cnt = 0;
	for (i = 2; cnt < n; i ++) 
		{
			if (is_prime(i)) 
				{
					cnt ++;
					summ += i;
				}
		}
	return summ;
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
	if (sscanf(argv[1], "%d", &n) != 1)
		{	
			printf("Can not read from %s \n", argv[1]);
			return 1;
		}
	res = sum_of_prime (n);
	printf ("%s : Task = %d Result = %e\n", argv[0], 10, res);
	return 0;
}