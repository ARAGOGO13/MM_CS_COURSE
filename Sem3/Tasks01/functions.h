#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#include "data.h"

int count_decreases(data *a, int n);

int binary_search(data *a, int n, const data &x);
void merge_arrays(data *a, data *b, data *c, int n, int m);
int dividing_array(data *a, int n, int m);
void bubble_sort(data *a, int n);
void find_min_sort(data *a, int n);
void linear_insert_sort(data *a, int n);
void binary_insert_sort(data *a, int n);
void merge_sort(data *a, data *b, int n);
void quick_sort(data *a, int n);
void tournament_sort(data *a, int n);





#endif // FUNCTIONS_H
