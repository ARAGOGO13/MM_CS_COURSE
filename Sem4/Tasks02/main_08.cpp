#include <cstdio>
#include <ctime>
#include "solve.h"

int main(int argc, char *argv[]) {
    int task = 8;
    if (argc != 5) {
        printf("Usage: %s fin fout s t\n", argv[0]);
        return 1;
    }

    const char *fin = argv[1];
    const char *fout = argv[2];
    const char *s = argv[3];
    const char *t = argv[4];

    int res = 0;
    clock_t start = clock();
    io_status status = task8(fin, fout, s, t, &res);
    double elapsed = double(clock() - start) / CLOCKS_PER_SEC;

    if (status == io_status::success) {
        printf("%s : Task = %d Result = %d Elapsed = %.2f\n", argv[0], task, res, elapsed);
        return 0;
    } else {
        switch (status) {
            case io_status::open:
                printf("Cannot open file\n");
                break;
            case io_status::format:
                printf("File format error\n");
                break;
            case io_status::memory:
                printf("Memory allocation error\n");
                break;
            case io_status::create:
                printf("Creation error\n");
                break;
            case io_status::eof:
                printf("Unexpected EOF\n");
                break;
            default:
                printf("Unknown error\n");
                break;
        }
        return 1;
    }
}
