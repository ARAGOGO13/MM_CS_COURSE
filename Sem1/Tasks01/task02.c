int printf (const char*, ...);
int scanf (const char*, ...);
unsigned int max_consecutive_ones (unsigned int);
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

unsigned int max_consecutive_ones(unsigned int n) 
{
	int mx = 0;
	int cnt = 0;
	while (n) 
		{
			if (n & 1) 
				{
					cnt += 1;
					if (cnt > mx) 
						{
							mx = cnt;
						}
				}
			else 
				{
					cnt = 0;
				}
			n >>= 1;
		}
	return mx;
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
	res = max_consecutive_ones (n);
	printf ("Result:\t");  
	print_bits (res);
	printf ("Task = %d Result = %u\t", 2, res);
	return 0;
}