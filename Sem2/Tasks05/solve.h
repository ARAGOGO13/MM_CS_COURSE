double max_row_sum (double *a, int n, int m);
double max_column_sum (double *a, int n, int m);
double residual_vector_sum (double *a, double *vec_x, double *vec_b, int n, int m);
double residual_vector_max (double *a, double *vec_x, double *vec_b, int n, int m);
double residual_matrix_row_sum (double *a, double *b, int n, int m);
double residual_matrix_column_sum (double *a, double *b, int n, int m);
double compute_laplacian (double *a, int n, int m);
double compute_laplacian_1 (double *a, int n, int m);
double compute_laplacian_2 (double *a, int n, int m);
double compute_laplacian_3 (double *a, int n, int m);
double compute_laplacian_4 (double *a, int n, int m);
void el_row_operation_2 (double *a, int n, int m, int i,  int j);
void el_column_operation_2 (double *a, int n, int m, int i,  int j);
double mx_to_a11 (double *a, int n, int m);


