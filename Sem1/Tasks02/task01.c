int printf (const char*, ...);
double machine_precision (void);

double machine_precision (void) 
{
	double epislon = 1;
	while (1. + epislon > 1.)
		{
			epislon /= 2;
		}
	return epislon;
}

int main(int argc, char* argv[]) 
{
	double res;
	(void) argc;
	res = machine_precision ();
	printf ("%s : Task = %d Result = %e\n", argv[0], 1, res);
	return 0;
}