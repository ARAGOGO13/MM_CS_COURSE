#include <cstdio>
#include <ctime>
#include <iostream>

#include "data.h"
#include "functions.h"
#include "io.h"

using namespace std;

int main(int argc, char *argv[]) {
    int n = 0, p = 0, s = 0, diff = 0;
    const char *file_name = nullptr;
    double t;
    int res;

    if (!((argc == 4 || argc == 5) && sscanf(argv[1], "%d", &n) == 1 && sscanf(argv[2], "%d", &p) == 1 &&
          sscanf(argv[3], "%d", &s) == 1)) {
        printf("Usage: %s n p s [file] \n", argv[0]);
        return 1;
    }

    if (p > n) {
        printf("Error: %d numbers cannot be output from an array of length %d \n", p, n);
        return 2;
    }

    if ((s == 0 && argc != 5) || (s != 0 && argc == 5)) {
        printf("Need to set s = 0 or add file_name \n");
        return 4;
    }

    if (argc == 5)
        file_name = argv[4];

    data *A = new data[n];
    if (!A) {
        printf("Not enough memory! \n");
        return 3;
    }

    if (file_name) {
        FILE *fa = fopen(file_name, "r");
        if (!fa) {
            printf("Error: Can not read from %s \n", file_name);
            delete[] A;
            return 3;
        }
        res = read_array_from_file(fa, A, n);
        fclose(fa);
        if (res != 0) {
            printf("Error: reading the file %s \n", file_name);
            delete[] A;
            return 3;
        }
    } else {
        if (init_array_by_formula(A, n, s) != 0) {
            printf("Error initializing array by formula s=%d\n", s);
            delete[] A;
            return 3;
        }
    }

    data::set_p(p);
    int rows = (n < p ? n : p);
    for (int i = 0; i < rows; ++i)
        A[i].print(stdout);

    clock_t t0 = clock();
    quick_sort(A, n);
    clock_t t1 = clock();
    t = double(t1 - t0) / CLOCKS_PER_SEC;

    data::set_p(p);
    cout << "New array:" << endl;
    rows = (n < p ? n : p);
    for (int i = 0; i < rows; ++i)
        A[i].print(stdout);

    diff = count_decreases(A, n);

    printf("%s : Task = %d Diff = %d Elapsed = %.2f\n", argv[0], 9, diff, t);

    delete[] A;
    return 0;
}
