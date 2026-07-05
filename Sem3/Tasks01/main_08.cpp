#include <cstdio>
#include <ctime>
#include <iostream>

#include "data.h"
#include "functions.h"
#include "io.h"

using namespace std;

int main(int argc, char *argv[]) {
    int n = 0, p = 0, s = 0;
    const char *file_name = nullptr;
    int res;

    if (!((argc == 4 || argc == 5) &&
          sscanf(argv[1], "%d", &n) == 1 &&
          sscanf(argv[2], "%d", &p) == 1 &&
          sscanf(argv[3], "%d", &s) == 1)) {
        printf("Usage: %s n p s [file] \n", argv[0]);
        return 1;
    }

    if (n <= 0) {
        printf("Error: n must be positive\n");
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

    if (argc == 5) file_name = argv[4];

    data *a = new data[n];
    data *b = new data[n];
    if (!a || !b) {
        printf("Not enough memory! \n");
        delete[] a;
        delete[] b;
        return 3;
    }

    if (file_name) {
        FILE *fa = fopen(file_name, "r");
        if (!fa) {
            printf("Error: Can not read from %s \n", file_name);
            delete[] a;
            delete[] b;
            return 3;
        }
        res = read_array_from_file(fa, a, n);
        fclose(fa);
        if (res != 0) {
            printf("Error: reading the file %s \n", file_name);
            delete[] a;
            delete[] b;
            return 3;
        }
    } else {
        if (init_array_by_formula(a, n, s) != 0) {
            printf("Error initializing array by formula s=%d\n", s);
            delete[] a;
            delete[] b;
            return 3;
        }
    }

    data::set_p(p);
    int rows = (n < p ? n : p);
    for (int i = 0; i < rows; ++i) a[i].print(stdout);


    clock_t t0 = clock();
    merge_sort(a, b, n);
    clock_t t1 = clock();
    double elapsed = double(t1 - t0) / CLOCKS_PER_SEC;

    data::set_p(p);
    cout << "New array:" << endl;
    rows = (n < p ? n : p);
    for (int i = 0; i < rows; ++i) a[i].print(stdout);

    int diff = count_decreases(a, n);

    printf("%s : Task = %d Diff = %d Elapsed = %.2f\n", argv[0], 8, diff, elapsed);

    delete[] a;
    delete[] b;
    return 0;
}
