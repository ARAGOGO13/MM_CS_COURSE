void swap(char *a, char *b);
int diff_calculation(double *a, int n, int(*cmp)(double, double));

int binary_search(char **a, int n, char *x, int (*cmp) (const char*, const char*));
void merge_arrays(char **a, char **b, char **c, int n, int m, int (*cmp) (const char*, const char*));
int dividing_array(char **a, int n, char *x, int (*cmp) (const char*, const char*));
void bubble_sort(char **a, int n, int (*cmp) (const char*, const char*));
void find_min_sort (char **a, int n, int (*cmp) (const char*, const char*));
void linear_insert_sort (char **a, int n, int (*cmp) (const char*, const char*));
void binary_insert_sort (char **a, int n, int (*cmp) (const char*, const char*));
void merge_sort (char **a, char **b, int n, int (*cmp) (const char*, const char*));
void quick_sort (char **a, int n, int (*cmp) (const char*, const char*));
void tournament_sort (char **a, int n, int (*cmp) (const char*, const char*));
