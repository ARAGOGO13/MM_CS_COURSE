#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#include "student.h"

int count_decreases(student *a, int n);
int binary_search(student *a, int n, const student &x);
void merge_arrays(student *a, student *b, student *c, int n, int m);
int dividing_array(student *a, int n, int m);
void bubble_sort(student *a, int n);
void find_min_sort(student *a, int n);
void linear_insert_sort(student *a, int n);
void binary_insert_sort(student *a, int n);
void merge_sort(student *a, student *b, int n);
void quick_sort(student *a, int n);
void tournament_sort(student *a, int n);

#endif
