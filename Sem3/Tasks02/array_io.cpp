#include "array_io.h"
#include <cstdio>

int read_array(student *a, int n, const char *filename) {
    FILE *fp;
    if (!(fp = fopen(filename, "r"))) {
        return ERROR_OPEN;
    }

    for (int i = 0; i < n; i++) {
        io_status res = a[i].read(fp);
        if (res != io_status::success) {
            fclose(fp);
            if (res == io_status::eof)
                return NOT_ENOUGH;
            return ERROR_READ;
        }
    }

    fclose(fp);
    return SUCCESS;
}

int init_array(student *a, int n, int s) {
    for (int i = 0; i < n; i++) {
        io_status res = a[i].init_by_formula(s, n, i + 1);
        if (res != io_status::success) {
            return -1;
        }
    }
    return 0;
}

void print_array(student *a, int n, int p) {
    int count = (p < n) ? p : n;
    for (int i = 0; i < count; i++) {
        a[i].print();
    }
}
