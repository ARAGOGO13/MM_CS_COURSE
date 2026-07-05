int printf (const char*, ...);
int sscanf (const char *, const char *, ...);
double f3 (double);

double f3 (double x) 
{
	if (x <= -1) 
		{
			return (x + 1)/(x - 3);
		}
	else if (-1 <= x && x <= 1) 
		{	
			return (-x - 1) / (x - 3);
		}
	else 
		{
			return 1;	
		}

}

int main(int argc, char* argv[]) 
{
	double x, res;
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
	res = f3 (x);
	printf ("%s : Task = %d Result = %e\n", argv[0], 3, res);
	return 0;
}