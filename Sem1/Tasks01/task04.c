int printf (const char*, ...);
int scanf (const char*, ...);
unsigned int max_distance_ones (unsigned int);
void print_bits (unsigned int);

void print_bits (unsigned int n)
{
	unsigned int bits = sizeof(n) * 8;
	int i;
	for (i = bits - 1; i >= 0; i--) 
		{
        int bit = (n >> i) & 1;
        printf ("%d", bit);
    }
    printf ("\n"); 
}

unsigned int max_distance_ones (unsigned int n) 
{
	unsigned int mx = 0;
	unsigned int cnt = 0;
	int flag = 0;
	while (n) 
		{
			if (n & 1 && flag && cnt > mx) 
				{
					mx = cnt;
				}
			if (flag) 
				{
					cnt ++;
				}
			if (n & 1 && !flag) 
				{
					flag = 1;
				}
			n >>= 1;
		}
	if (mx > 0) 
		{
			return mx;
		}
	return mx > 0 ? mx : (unsigned int) - 1; 
}

int main(void) 
{
	unsigned int n, res;
	printf ("Input:\t");
	if (scanf ("%u", &n) != 1)
		{
			printf ("Usage: number\n");
			return 1;
		}


	print_bits (n);
	res = max_distance_ones (n);
	printf ("Result: \t");  
	print_bits (res);
	printf ("Task = %d Result = %u\n", 4, res);
	return 0;
}