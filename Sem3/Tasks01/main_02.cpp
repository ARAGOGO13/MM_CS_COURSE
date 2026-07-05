// main_02.cpp
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
    if (!(7 <= argc && argc <= 9)) {
        printf("Usage: %s nA pA sA [nB | fileA] pB sB [fileB]\n", argv[0]);
        return 1;
    }

    int nA = 0, nB = 0;
    int pA = 0, pB = 0;
    int sA = 0, sB = 0;
    char *file_name_a = nullptr;
    char *file_name_b = nullptr;

    if (sscanf(argv[1], "%d", &nA) != 1 || sscanf(argv[2], "%d", &pA) != 1 || sscanf(argv[3], "%d", &sA) != 1) {
        printf("Error: invalid numeric arguments\n");
        printf("Usage: %s nA pA sA [nB | fileA] pB sB [fileB]\n", argv[0]);
        return 1;
    }

    if (pA > nA) {
        printf("Error: %d numbers cannot be output from an array of length %d\n", pA, nA);
        return 2;
    }

    if (sscanf(argv[4], "%d", &nB) != 1) {
        file_name_a = argv[4];
        if (!(sscanf(argv[5], "%d", &nB) == 1 && sscanf(argv[6], "%d", &pB) == 1 && sscanf(argv[7], "%d", &sB) == 1)) {
            printf("Usage: %s nA pA sA [nB | fileA] pB sB [fileB]\n", argv[0]);
            return 1;
        }
        if (argc == 9)
            file_name_b = argv[8];
    } else {
        if (!(sscanf(argv[5], "%d", &pB) == 1 && sscanf(argv[6], "%d", &sB) == 1)) {
            printf("Usage: %s nA pA sA [nB | fileA] pB sB [fileB]\n", argv[0]);
            return 1;
        }
        if (argc == 8)
            file_name_b = argv[7];
    }

    if (pB > nB) {
        printf("Error: %d numbers cannot be output from an array of length %d\n", pB, nB);
        return 2;
    }

    cout << nA << " " << pA << " " << sA << " " << nB << " " << pB << " " << sB << endl;

    data *A = new data[nA];
    data *B = new data[nB];
    data *C = new data[nA + nB];
    if (!A || !B || !C) {
        printf("Error: Not enough memory!\n");
        delete[] A;
        delete[] B;
        delete[] C;
        return 3;
    }

    if (file_name_a) {
        FILE *fa = fopen(file_name_a, "r");
        if (!fa) {
            printf("Error: Cannot open %s\n", file_name_a);
            delete[] A;
            delete[] B;
            delete[] C;
            return 3;
        }
        if (read_array_from_file(fa, A, nA) != 0) {
            printf("Error reading array A from %s\n", file_name_a);
            fclose(fa);
            delete[] A;
            delete[] B;
            delete[] C;
            return 3;
        }
        fclose(fa);
    } else {
        if (init_array_by_formula(A, nA, sA) != 0) {
            printf("Error initializing array A by formula\n");
            delete[] A;
            delete[] B;
            delete[] C;
            return 3;
        }
    }

    if (file_name_b) {
        FILE *fb = fopen(file_name_b, "r");
        if (!fb) {
            printf("Error: Cannot open %s\n", file_name_b);
            delete[] A;
            delete[] B;
            delete[] C;
            return 3;
        }

        if (read_array_from_file(fb, B, nB) != 0) {
            printf("Error reading array B from %s\n", file_name_b);
            fclose(fb);
            delete[] A;
            delete[] B;
            delete[] C;
            return 3;
        }
        fclose(fb);
    } else {
        if (init_array_by_formula(B, nB, sB) != 0) {
            printf("Error initializing array B by formula\n");
            delete[] A;
            delete[] B;
            delete[] C;
            return 3;
        }
    }

    cout << "Array A:" << endl;
    data::set_p(pA);
    int rowsA = (nA < pA ? nA : pA);
    for (int i = 0; i < rowsA; ++i)
        A[i].print(stdout);

    cout << "Array B:" << endl;
    data::set_p(pB);
    int rowsB = (nB < pB ? nB : pB);
    for (int i = 0; i < rowsB; ++i)
        B[i].print(stdout);

    clock_t t0 = clock();
    merge_arrays(A, B, C, nA, nB);
    clock_t t1 = clock();
    double elapsed = double(t1 - t0) / CLOCKS_PER_SEC;

    data::set_p(pA + pB);
    cout << "New array:" << endl;
    int rowsC = ((nA + nB) < (pA + pB) ? (nA + nB) : (pA + pB));
    for (int i = 0; i < rowsC; ++i)
        C[i].print(stdout);

    int diff = count_decreases(C, nA + nB);

    printf("%s : Task = %d Diff = %d Elapsed = %.2f\n", argv[0], 2, diff, elapsed);

    delete[] A;
    delete[] B;
    delete[] C;
    return 0;
}
