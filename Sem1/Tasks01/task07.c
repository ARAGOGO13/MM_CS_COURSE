int printf (const char*, ...);
int scanf (const char*, ...);
unsigned int switch_adjacent_zeros (unsigned int);
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

unsigned int switch_adjacent_zeros (unsigned int n) 
{
	unsigned int res = 0;
	unsigned int previous_bit = 1;
	int i;
	int bits = find_num_size(n);

    for (i = 0; i < bits; i++) 
	    {
	    	unsigned int next_bit = (n >> (i + 1)) & 1;
	        unsigned int current_bit = (n >> i) & 1;
	        if (current_bit == 0 && previous_bit == 0 && next_bit == 0)
		        {
		        	res += (1 << i);
		        }
		    else 
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
	res = switch_adjacent_zeros (n);
	printf ("Result: \t");  
	print_bits (res);
	printf ("Task = %d Result = %u\n", 7, res);
	return 0;
}