#include <cstdio>
#include <ctime>
#include "solve.h"

int main(int argc, char *argv[]) {
    int task = 1;
    if (argc != 6) {
        printf("Usage: %s fin fout s t m\n", argv[0]);
        return 1;
    }
    const char *fin = argv[1];
    const char *fout = argv[2];
    const char *s = argv[3];
    const char *t = argv[4];
    int m;
    if (sscanf(argv[5], "%d", &m) != 1) {
        printf("Error: m must be an integer\n");
        return 1;
    }
    int res = 0;
    clock_t start = clock();
    io_status status = task1(fin, fout, s, t, m, &res);
    double elapsed = double(clock() - start) / CLOCKS_PER_SEC;

    if (status == io_status::success) {
        printf("%s : Task = %d Result = %d Elapsed = %.2f\n", argv[0], task, res, elapsed);
        return 0;
    } else {
        switch (status) {
            case io_status::open:
                printf("Cannot open file \n");
                break;
            case io_status::format:
                printf("File format error \n");
                break;
            case io_status::memory:
                printf("Memory allocation error \n");
                break;
            case io_status::create:
                printf("Creation error \n");
                break;
            case io_status::eof:
                printf("Unexpected EOF \n");
                break;
            default:
                printf("Unknown error \n");
        }
        return 1;
    }
}
