#include <cstdio>
#include <ctime>
#include <iostream>
#include <new>

#include "array_io.h"
#include "functions.h"
#include "student.h"

using namespace std;

int main(int argc, char *argv[]) {
    int n = 0, p = 0, s = 0;
    const char *file_name = nullptr;
    const char *x_name = nullptr;
    int x_value = 0;

    if (!((argc == 6 || argc == 7) && sscanf(argv[3], "%d", &n) == 1 && sscanf(argv[4], "%d", &p) == 1 &&
          sscanf(argv[5], "%d", &s) == 1)) {
        printf("Usage: %s x_name x_value n p s [file]\n", argv[0]);
        return 1;
    }

    x_name = argv[1];
    if (sscanf(argv[2], "%d", &x_value) != 1) {
        printf("Error: x_value must be integer\n");
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
    if ((s == 0 && argc != 7) || (s != 0 && argc == 7)) {
        printf("Need to set s = 0 or add file_name\n");
        return 4;
    }

    if (argc == 7)
        file_name = argv[6];

    student x;
    io_status ret = x.set(x_name, x_value);
    if (ret != io_status::success) {
        printf("Error initializing student x\n");
        return 1;
    }

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

    print_array(A, n, p);

    clock_t t0 = clock();
    int result = task07(A, n, x);
    clock_t t1 = clock();
    double elapsed = double(t1 - t0) / CLOCKS_PER_SEC;

    cout << "New array:" << endl;
    print_array(A, result, p);

    printf("%s : Task = %d Result = %d Elapsed = %.2f\n", argv[0], 7, result, elapsed);

    delete[] A;
    return 0;
}
