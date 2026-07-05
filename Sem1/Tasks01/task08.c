int printf (const char*, ...);
int scanf (const char*, ...);
unsigned int count_adjacent_zeros (unsigned int);
void print_bits (unsigned int);
unsigned int find_num_size (unsigned int n);

unsigned int find_num_size (unsigned int n) 
{
	unsigned int size = 0;
	while	(n) 
		{	
			size ++;
			n >>= 1;
		}
	return size;
}

void print_bits (unsigned int n)
{
	int bits = sizeof(n) * 8;

	for (int i = bits - 1; i >= 0; i--) 
		{
        int bit = (n >> i) & 1;
        printf ("%d", bit);
    }
    printf ("\n"); 
}

unsigned int count_adjacent_zeros (unsigned int n) 
{
	unsigned int previous_bit = 1;
	int i;
	unsigned int count = 0;
	int bits = find_num_size(n);

    for (i = 0; i < bits - 1; i++) 
	    {
	    	unsigned int next_bit = (n >> (i + 1)) & 1;
	        unsigned int current_bit = (n >> i) & 1;
	        if (current_bit == 1 && previous_bit == 0 && next_bit == 0)
		        {
		        	count ++;
		        }
	        previous_bit = current_bit;
	    }
	return count;
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
	res = count_adjacent_zeros (n);
	printf ("Result: \t");  
	print_bits (res);
	printf ("Task = %d Result = %u\n", 8, res);
	return 0;
}