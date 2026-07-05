#include "io.h"
#include <cstdio>

int f_formula(int s, int n, int i) {
    switch (s) {
        case 1:
            return i;
        case 2:
            return n - i;
        case 3:
            return i / 2;
        case 4:
            return n - i / 2;
        default:
            return 0;
    }
}

int read_array_from_file(FILE *fp, data *arr, int n) {
    if (!fp)
        return -1;
    for (int i = 0; i < n; ++i) {
        if (arr[i].read(fp) != 0)
            return -1;
    }
    return 0;
}

int init_array_by_formula(data *arr, int n, int s) {
    for (int i = 0; i < n; ++i) {
        arr[i].init(s, n, i + 1);
    }
    return 0;
}
