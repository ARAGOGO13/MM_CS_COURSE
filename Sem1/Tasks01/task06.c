int printf (const char*, ...);
int scanf (const char*, ...);
unsigned int switch_adjacent_ones (unsigned int);
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

unsigned int switch_adjacent_ones (unsigned int n) 
{
	unsigned int previous_bit = 0;
	int i;
	int bits = find_num_size(n);
	unsigned int res = 1 << (bits - 1);
    for (i = 0; i < bits - 1; i++) 
	    {
	    		unsigned int next_bit = (n >> (i + 1)) & 1;
	        unsigned int current_bit = (n >> i) & 1;
	        if (!(current_bit == 1 && previous_bit == 1 && next_bit == 1)) 
		        {
		            res += (current_bit << i);
		        }
	        previous_bit = current_bit;
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
	res = switch_adjacent_ones (n);
	printf ("Result: \t");  
	print_bits (res);
	printf ("Task = %d Result = %u\n", 6, res);
	return 0;
}