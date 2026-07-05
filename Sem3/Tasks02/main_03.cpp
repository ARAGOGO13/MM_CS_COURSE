#include <cstdio>
#include <ctime>
#include <iostream>

#include "array_io.h"
#include "functions.h"
#include "student.h"

using namespace std;

int main(int argc, char *argv[]) {
    int m_idx = 0, n = 0, p = 0, s = 0;
    const char *afile = nullptr;

    if (!((argc == 5 || argc == 6) && sscanf(argv[1], "%d", &m_idx) == 1 && sscanf(argv[2], "%d", &n) == 1 &&
          sscanf(argv[3], "%d", &p) == 1 && sscanf(argv[4], "%d", &s) == 1)) {
        printf("Usage: %s m n p s [file]\n", argv[0]);
        return 1;
    }

    if (n <= 0) {
        printf("Error: n must be positive\n");
        return 1;
    }
    if (m_idx < 0 || m_idx >= n) {
        printf("Error: m must be between 0 and %d\n", n - 1);
        return 1;
    }
    if (p < 0) {
        printf("Error: p must be non-negative\n");
        return 1;
    }
    if ((s == 0 && argc != 6) || (s != 0 && argc == 6)) {
        printf("Need to set s = 0 or add file_name\n");
        return 4;
    }

    if (argc == 6)
        afile = argv[5];

    student *A = new student[n];
    if (!A) {
        printf("Not enough memory!\n");
        return 3;
    }

    if (afile) {
        int read_result = read_array(A, n, afile);
        if (read_result != SUCCESS) {
            if (read_result == ERROR_OPEN) {
                printf("Error: Cannot open %s\n", afile);
            } else if (read_result == ERROR_READ) {
                printf("Error reading array from %s\n", afile);
            } else if (read_result == NOT_ENOUGH) {
                printf("Error: Not enough data in file %s\n", afile);
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
    int pos = dividing_array(A, n, m_idx);
    clock_t t1 = clock();
    double elapsed = double(t1 - t0) / CLOCKS_PER_SEC;

    cout << "New array:" << endl;
    print_array(A, n, p);
    printf("%s : Task = %d Res = %d Elapsed = %.2f\n", argv[0], 3, pos, elapsed);

    delete[] A;
    return 0;
}
