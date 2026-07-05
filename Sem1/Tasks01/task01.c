int printf (const char*, ...);
int scanf (const char*, ...);
unsigned int reverse_number (unsigned int);
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

unsigned int reverse_number (unsigned int n) 
{
	unsigned int res = 0;
	while (n) 
		{
			res <<= 1;
			res += (n & 1);
			n >>= 1;
		}
	return res;
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
	res = reverse_number (n);
	printf ("Result: \t");  
	print_bits (res);
	printf ("Task = %d Result = %u\n", 1, res);
	return 0;
}