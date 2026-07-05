#ifndef ARRAY_IO_H
#define ARRAY_IO_H

#include "student.h"

#define ERROR_OPEN (-1)
#define ERROR_READ (-2)
#define NOT_ENOUGH (-3)
#define SUCCESS 0

extern int read_array(student *a, int n, const char *filename);
extern int init_array(student *a, int n, int s);
extern void print_array(student *a, int n, int p);

#endif
