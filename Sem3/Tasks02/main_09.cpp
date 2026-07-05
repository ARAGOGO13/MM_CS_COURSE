#include <cstdio>
#include <ctime>
#include <iostream>

#include "array_io.h"
#include "functions.h"
#include "student.h"

using namespace std;

int main(int argc, char *argv[]) {
    int n = 0, p = 0, s = 0;
    const char *file_name = nullptr;

    if (!((argc == 4 || argc == 5) && sscanf(argv[1], "%d", &n) == 1 && sscanf(argv[2], "%d", &p) == 1 &&
          sscanf(argv[3], "%d", &s) == 1)) {
        printf("Usage: %s n p s [file]\n", argv[0]);
        return 1;
    }

    if (n <= 0) {
        printf("Error: n must be positive\n");
        return 1;
    }
    if (p < 0) {
        printf("Error: p must be non-negative\n");
        return 1;
    }
    if ((s == 0 && argc != 5) || (s != 0 && argc == 5)) {
        printf("Need to set s = 0 or add file_name\n");
        return 4;
    }

    if (argc == 5)
        file_name = argv[4];

    student *A = new student[n];
    if (!A) {
        printf("Not enough memory!\n");
        return 3;
    }

    if (file_name) {
        int read_result = read_array(A, n, file_name);
        if (read_result != SUCCESS) {
            if (read_result == ERROR_OPEN) {
                printf("Error: Cannot open %s\n", file_name);
            } else if (read_result == ERROR_READ) {
                printf("Error reading array from %s\n", file_name);
            } else if (read_result == NOT_ENOUGH) {
                printf("Error: Not enough data in file %s\n", file_name);
            }
            delete[] A;
            return 3;
        }
    } else {
        if (init_array(A, n, s) != 0) {
            printf("Error initializing array by formula s=%d\n", s);
            delete[] A;
            return 3;
        }
    }

    cout << "Array A:" << endl;
    print_array(A, n, p);
    clock_t t0 = clock();
    quick_sort(A, n);
    clock_t t1 = clock();
    double elapsed = double(t1 - t0) / CLOCKS_PER_SEC;

    cout << "New array:" << endl;
    print_array(A, n, p);

    int diff = count_decreases(A, n);
    printf("%s : Task = %d Diff = %d Elapsed = %.2f\n", argv[0], 9, diff, elapsed);

    delete[] A;
    return 0;
}
