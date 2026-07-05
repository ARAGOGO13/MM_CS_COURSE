int is_symmetric (double *a, int n);
int matrix_trace (double *a, int n);
void transposed_matrix (double *a, int n);
void symmetric_part (double *a, int n);
void antisymmetric_part (double *a, int n);
void el_row_operation_2 (double *a, int n, int m, int i,  int j);
void el_column_operation_2 (double *a, int n, int m, int i,  int j);
void el_row_operation_1 (double *a, int n, int m, int i,  int j, double g);
void matrix_vector_multiplication (double *a, double *vec_b, int m, double *vec_c, int n);
void matrix_multiplication (double *a, int n, int m, double *b, int k, double *c);



