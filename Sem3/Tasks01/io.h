#ifndef IO_H
#define IO_H

#include <stdio.h>
#include "data.h"

int f_formula(int s, int n, int i);
int read_array_from_file(FILE *fp, data *arr, int n);
int init_array_by_formula(data *arr, int n, int s);

#endif // IO_H
