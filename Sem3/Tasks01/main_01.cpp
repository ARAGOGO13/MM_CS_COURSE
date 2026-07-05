#include <cstdio>
#include <ctime>
#include <iostream>

#include "data.h"
#include "functions.h"
#include "io.h"

using namespace std;

int main(int argc, char *argv[]) {

    int n = 0, p = 0, s = 0;
    int position = 0;
    int task = 1;
    const char *x_filename = 0;
    FILE *fx = 0;
    FILE *fa = 0;

    if (!((argc == 5 || argc == 6) && sscanf(argv[2], "%d", &n) == 1 && sscanf(argv[3], "%d", &p) == 1 &&
          sscanf(argv[4], "%d", &s) == 1)) {
        printf("Usage: %s x n p s [file] \n", argv[0]);
        return 1;
    }

    x_filename = argv[1];

    if (n <= 0) {
        printf("Error: n must be positive\n");
        return 1;
    }
    if (p < 0) {
        printf("Error: p must be non-negative\n");
        return 1;
    }

    if ((s == 0 && argc != 6) || (s != 0 && argc == 6)) {
        printf("Error: Need to set s = 0 or add file_name \n");
        return 1;
    }

    data::set_p(p);

    data x;
    fx = fopen(x_filename, "r");
    if (!fx) {
        printf("Error opening x file\n");
        return 1;
    }
    if (x.read(fx) != 0) {
        printf("Error reading object x from %s\n", x_filename);
        fclose(fx);
        return 1;
    }
    fclose(fx);

    data *arr = new data[n];

    if (s == 0) {
        const char *arr_filename = argv[5];
        fa = fopen(arr_filename, "r");
        if (!fa) {
            printf("Error opening array file\n");
            delete[] arr;
            return 1;
        }
        if (read_array_from_file(fa, arr, n) != 0) {
            printf("Error reading array from %s\n", arr_filename);
            fclose(fa);
            delete[] arr;
            return 1;
        }
        fclose(fa);
    } else {
        init_array_by_formula(arr, n, s);
    }

    cout << "Array A:" << endl;
    int len = (n < data::get_p() ? n : data::get_p());
    for (int i = 0; i < len; ++i) {
        arr[i].print(stdout);
    }

    cout << "Object X:" << endl;
    x.print(stdout);

    clock_t start = clock();
    position = binary_search(arr, n, x);
    clock_t end = clock();
    double elapsed = double(end - start) / CLOCKS_PER_SEC;

    printf("%s : Task = %d Res = %d Elapsed = %.2f\n", argv[0], task, position, elapsed);

    delete[] arr;
    return 0;
}
