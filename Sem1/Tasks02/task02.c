int printf (const char*, ...);
int sscanf (const char *, const char *, ...);
int binary_order (double);

int binary_order (double x) 
{
	double two = 1.;
	int cnt = 0;
	if (x >= 1) {
		while (!(two <= x && x < two * 2))
		{
			two *= 2;
			cnt ++;
		}
	}
	
	while (!(two <= x && x < two * 2))
		{
			two /= 2;
			cnt --;
		}

	return cnt;
}

int main(int argc, char* argv[]) 
{
	double x;
	int res;
	if (argc != 2)
		{	
			printf("Can not read from %s \n", argv[0]);
			return 1;
		}
	if ((sscanf(argv[1], "%lf", &x)) != 1)
		{	
			printf("Can not read from %s \n", argv[1]);
			return 1;
		}
	res = binary_order (x);
	printf ("%s : Task = %d Result = %d\n", argv[0], 2, res);
	return 0;
}