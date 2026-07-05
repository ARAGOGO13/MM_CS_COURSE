void swap(double *a, double *b);
int diff_calculation(double *a, int n, int(*cmp)(double, double));

int binary_search(double *a, int n, double x, int(*cmp)(double, double));
void merge_arrays(double *a, double *b, double *c, int n, int m, int(*cmp)(double, double));
int dividing_array(double *a, int n, double x, int(*cmp)(double, double));
void bubble_sort(double *a, int n, int(*cmp)(double, double));
void find_min_sort(double *a, int n, int(*cmp)(double, double));
void linear_insert_sort(double *a, int n, int(*cmp)(double, double));
void binary_insert_sort(double *a, int n, int(*cmp)(double, double));
void merge_sort(double *a, double *b, int n, int(*cmp)(double, double));
void quick_sort(double *a, int n, int(*cmp)(double, double));
void tournament_sort(double *arr, int n, int(*cmp)(double, double));