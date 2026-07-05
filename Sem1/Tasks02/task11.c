int printf (const char*, ...);
int sscanf (const char *, const char *, ...);
int is_prime (int);
int n_prime (int);

int is_prime (int n) 
{
	int i;
	for (i = 2; i * i <= n; i ++) 
		{
			if (n % i == 0) return 0;
		}
	return 1;
}

int n_prime (int n) 
{
	int cnt = 0;
	int i, res;
	for (i = 2; cnt < n; i ++) 
		{
			if (is_prime(i)) 
				{
					res = i;
					cnt ++;
				}
		}
	return res;
}

int main(int argc, char* argv[]) 
{
	int n, res;
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
	res = n_prime (n);
	printf ("%s : Task = %d Result = %d\n", argv[0], 11, res);
	return 0;
}