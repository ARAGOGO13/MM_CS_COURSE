// main_03.cpp
#include <cstdio>
#include <cstdlib>
#include <ctime>
#include <iostream>
#include <new>

#include "data.h"
#include "functions.h"
#include "io.h"

using namespace std;

int main(int argc, char *argv[]) {
    int m_idx = 0, n = 0, p = 0, s = 0;
    const char *afile = nullptr;
    int res;

    if (!((argc == 5 || argc == 6) && sscanf(argv[1], "%d", &m_idx) == 1 && sscanf(argv[2], "%d", &n) == 1 &&
          sscanf(argv[3], "%d", &p) == 1 && sscanf(argv[4], "%d", &s) == 1)) {
        printf("Usage: %s m n p s [file]\n", argv[0]);
        return 1;
    }

    if (n <= 0) {
        printf("Error: n must be positive\n");
        return 1;
    }
    if (m_idx < 0) {
        printf("Error: m must be > 0\n");
        return 1;
    }
    if (p > n) {
        printf("Error: %d numbers cannot be output from an array of length %d\n", p, n);
        return 2;
    }
    if ((s == 0 && argc != 6) || (s != 0 && argc == 6)) {
        printf("Need to set s = 0 or add file_name\n");
        return 4;
    }

    if (argc == 6)
        afile = argv[5];

    data *A = new data[n];
    if (!A) {
        printf("Not enough memory!\n");
        return 3;
    }

    if (afile) {
        FILE *fa = fopen(afile, "r");
        if (!fa) {
            printf("Error: Can not read from %s\n", afile);
            delete[] A;
            return 3;
        }
        res = read_array_from_file(fa, A, n);
        fclose(fa);
        if (res != 0) {
            printf("Error: reading array from %s\n", afile);
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

    cout << "Array A:" << endl;
    int rows = (n < p ? n : p);
    for (int i = 0; i < rows; ++i)
        A[i].print(stdout);

    clock_t t0 = clock();
    int pos = dividing_array(A, n, m_idx);
    clock_t t1 = clock();
    double elapsed = double(t1 - t0) / CLOCKS_PER_SEC;

    printf("%s : Task = %d Res = %d Elapsed = %.2f\n", argv[0], 3, pos, elapsed);

    delete[] A;
    return 0;
}
