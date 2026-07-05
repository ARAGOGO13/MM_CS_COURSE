#define ERROR_OPEN (-1)
#define ERROR_READ (-2)
#define EMPTY_FILE (-3)
#define NOT_ENOUGH_ELEMENTS (-4)
#define EPS (1e-100)
#define SUCCESS (0)


double f(int s, int n, int i);
int read_array(double *a, int n, const char *name);
int init_array(double *a, int n, int s);
void print_array(const double *a, int n, int p);