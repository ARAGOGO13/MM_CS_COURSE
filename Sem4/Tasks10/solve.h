#ifndef SOLVE_H
#define SOLVE_H
#include "io.h"
#include <string>

io_status task01(const char *a, const char *b, int &r);
io_status task02(const char *a, const char *b, int &r);
io_status task03(const char *a, const char *b, int &r);
io_status task04(const char *a, const char *b, int &r);
io_status task05(const char *a, const char *b, const char *c, std::string &t,
                 int &r);
io_status task06(const char *a, const char *b, const char *c, std::string &t,
                 int &r);
#endif