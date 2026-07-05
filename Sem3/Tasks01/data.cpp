#include "data.h"
#include <cstdio>
#include <iostream>
#include <ostream>

#include "io.h"

using namespace std;

int data::p = 0;

data::data() : m(0) {
    for (int i = 0; i < M; ++i)
        a[i] = 0.0;
}

void data::print(FILE *fp) const {
    int len = get_M(), i = 0;
    fprintf(fp, "%d", m);
    if (len > p)
        len = p;

    for (i = 0; i < len; ++i) {
        fprintf(fp, " %le", a[i]);
    }
    fprintf(fp, "\n");
}

int data::read(FILE *fp) {
    int i = 0, len = 0;
    if(fscanf(fp, "%d", & m) != 1)
    {
        return -1;
    }
    len = get_M();
    for(i=0;i<len;i++)
    {
        if (fscanf(fp, "%lf", a+i) != 1)
        {
            return -1;
        }
    }
    return 0;
}

void data::init(int s, int n, int i) {
    {
        int len = get_M(), j = 0;
        m = f_formula(s, n, i);
        for (j = 0; j < len; j++) {
            a[j] = 0;
        }
    }
}
