#include <cstdio>
#include <ctime>
#include <iostream>

#include "array_io.h"
#include "functions.h"
#include "student.h"

using namespace std;

int main(int argc, char *argv[]) {
    if (!(7 <= argc && argc <= 9)) {
        printf("Usage: %s nA pA sA [nB | fileA] pB sB [fileB]\n", argv[0]);
        return 1;
    }

    int nA = 0, nB = 0;
    int pA = 0, pB = 0;
    int sA = 0, sB = 0;
    const char *file_name_a = nullptr;
    const char *file_name_b = nullptr;

    if (sscanf(argv[1], "%d", &nA) != 1 || sscanf(argv[2], "%d", &pA) != 1 || sscanf(argv[3], "%d", &sA) != 1) {
        printf("Error: invalid numeric arguments\n");
        printf("Usage: %s nA pA sA [nB | fileA] pB sB [fileB]\n", argv[0]);
        return 1;
    }

    if (nA <= 0) {
        printf("Error: nA must be positive\n");
        return 1;
    }
    if (pA < 0) {
        printf("Error: pA must be non-negative\n");
        return 1;
    }
    if ((sA == 0 && argc < 8) || (sA != 0 && argc >= 8 && sscanf(argv[4], "%d", &nB) != 1)) {
        printf("Error: For sA=0, fileA must be provided\n");
        return 1;
    }

    if (sscanf(argv[4], "%d", &nB) != 1) {
        file_name_a = argv[4];
        if (argc < 8 || sscanf(argv[5], "%d", &nB) != 1 || sscanf(argv[6], "%d", &pB) != 1 ||
            sscanf(argv[7], "%d", &sB) != 1) {
            printf("Usage: %s nA pA sA [nB | fileA] pB sB [fileB]\n", argv[0]);
            return 1;
        }
        if (argc == 9)
            file_name_b = argv[8];
    } else {
        if (argc < 7 || sscanf(argv[5], "%d", &pB) != 1 || sscanf(argv[6], "%d", &sB) != 1) {
            printf("Usage: %s nA pA sA [nB | fileA] pB sB [fileB]\n", argv[0]);
            return 1;
        }
        if (argc == 8)
            file_name_b = argv[7];
    }

    if (nB <= 0) {
        printf("Error: nB must be positive\n");
        return 1;
    }
    if (pB < 0) {
        printf("Error: pB must be non-negative\n");
        return 1;
    }
    if (sB == 0 && file_name_b == nullptr) {
        printf("Error: For sB=0, fileB must be provided\n");
        return 1;
    }
    if (sB != 0 && file_name_b != nullptr) {
        printf("Error: For sB != 0, fileB should not be provided\n");
        return 1;
    }

    if (pA > nA) {
        printf("Error: %d numbers cannot be output from an array of length %d\n", pA, nA);
        return 2;
    }
    if (pB > nB) {
        printf("Error: %d numbers cannot be output from an array of length %d\n", pB, nB);
        return 2;
    }

    student *A = new student[nA];
    student *B = new student[nB];
    student *C = new student[nA + nB];
    if (!A || !B || !C) {
        printf("Error: Not enough memory!\n");
        delete[] A;
        delete[] B;
        delete[] C;
        return 3;
    }

    if (file_name_a) {
        int read_result = read_array(A, nA, file_name_a);
        if (read_result != SUCCESS) {
            if (read_result == ERROR_OPEN) {
                printf("Error: Cannot open %s\n", file_name_a);
            } else if (read_result == ERROR_READ) {
                printf("Error reading array A from %s\n", file_name_a);
            } else if (read_result == NOT_ENOUGH) {
                printf("Error: Not enough data in file %s\n", file_name_a);
            }
            delete[] A;
            delete[] B;
            delete[] C;
            return 3;
        }
    } else {
        if (init_array(A, nA, sA) != 0) {
            printf("Error initializing array A by formula\n");
            delete[] A;
            delete[] B;
            delete[] C;
            return 3;
        }
    }

    if (file_name_b) {
        int read_result = read_array(B, nB, file_name_b);
        if (read_result != SUCCESS) {
            if (read_result == ERROR_OPEN) {
                printf("Error: Cannot open %s\n", file_name_b);
            } else if (read_result == ERROR_READ) {
                printf("Error reading array B from %s\n", file_name_b);
            } else if (read_result == NOT_ENOUGH) {
                printf("Error: Not enough data in file %s\n", file_name_b);
            }
            delete[] A;
            delete[] B;
            delete[] C;
            return 3;
        }
    } else {
        if (init_array(B, nB, sB) != 0) {
            printf("Error initializing array B by formula\n");
            delete[] A;
            delete[] B;
            delete[] C;
            return 3;
        }
    }

    cout << "Array A:" << endl;
    print_array(A, nA, pA);

    cout << "Array B:" << endl;
    print_array(B, nB, pB);

    clock_t t0 = clock();
    merge_arrays(A, B, C, nA, nB);
    clock_t t1 = clock();
    double elapsed = double(t1 - t0) / CLOCKS_PER_SEC;

    cout << "New array:" << endl;
    print_array(C, nA + nB, pA + pB);

    int diff = count_decreases(C, nA + nB);

    printf("%s : Task = %d Diff = %d Elapsed = %.2f\n", argv[0], 2, diff, elapsed);

    delete[] A;
    delete[] B;
    delete[] C;
    return 0;
}
