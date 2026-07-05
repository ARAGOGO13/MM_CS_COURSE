#include <cstdio>
#include <ctime>
#include <iostream>

#include "array_io.h"
#include "functions.h"
#include "student.h"
#

using namespace std;

int main(int argc, char *argv[]) {
    int n = 0, p = 0, s = 0;
    int task = 1;
    int pos = 0;
    const char *x_name = nullptr;
    int x_value = 0;

    if (!(argc == 6 || argc == 7) || sscanf(argv[3], "%d", &n) != 1 || sscanf(argv[4], "%d", &p) != 1 ||
        sscanf(argv[5], "%d", &s) != 1) {
        printf("Usage: %s x_name x_value n p s [filename]\n", argv[0]);
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
        printf("Error: Need to set s = 0 or add filename\n");
        return 1;
    }

    student x;
    io_status ret = x.set(x_name, x_value);
    if (ret != io_status::success) {
        printf("Error initializing student x\n");
        return 1;
    }

    student *arr = new (std::nothrow) student[n];
    if (!arr) {
        printf("Not enough memory!\n");
        return 1;
    }

    if (s == 0) {
        const char *filename = argv[6];
        int read_result = read_array(arr, n, filename);
        if (read_result != SUCCESS) {
            printf("Error reading array from file\n");
            delete[] arr;
            return 1;
        }
    } else {
        if (init_array(arr, n, s) != 0) {
            printf("Error initializing array by formula\n");
            delete[] arr;
            return 1;
        }
    }

    cout << "X:" << endl;
    x.print();

    cout << "Array A:" << endl;
    print_array(arr, n, p);

    clock_t start = clock();
    pos = binary_search(arr, n, x);
    clock_t end = clock();
    double elapsed = double(end - start) / CLOCKS_PER_SEC;

    printf("%s : Task = %d Res = %d Elapsed = %.2f\n", argv[0], task, pos, elapsed);

    delete[] arr;
    return 0;
}
