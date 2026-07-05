#include <cstdio>
#include <ctime>
#include <cstdlib>
#include "tree.h"

int main(int argc, char *argv[]) {
    int p = 0;
    int k = 0;
    const char *filename = nullptr;

    if (!(argc == 4 && sscanf(argv[1], "%d", &p) == 1 && sscanf(argv[3], "%d", &k) == 1)) {
        printf("Usage: %s p filename k\n", argv[0]);
        return 1;
    }

    if (p <= 0) {
        printf("Error: p must be positive\n");
        return 1;
    }
    filename = argv[2];

    tree my_tree;
    io_status status = read_file(&my_tree, filename);
    if (status != io_status::success) {
        switch (status) {
            case io_status::success:
                break;
            case io_status::eof:
                printf("Can not open %s\n", filename);
                break;
            case io_status::format:
                printf("Can not read %s\n", filename);
                break;
            case io_status::memory:
                printf("Not enough memory\n");
                break;
            default:
                printf("Read error\n");
                break;
        }
        return 1;
    }

    printf("Initial tree:\n");
    my_tree.print(p);

    for (int task = 1; task <= 7; ++task) {
        clock_t start_time = clock();
        int result = 0;
        switch (task) {
            case 1:
                result = my_tree.task01(k);
                break;
            case 2:
                result = my_tree.task02(k);
                break;
            case 3:
                result = my_tree.task03(k);
                break;
            case 4:
                result = my_tree.task04(k);
                break;
            case 5:
                result = my_tree.task05(k);
                break;
            case 6:
                result = my_tree.task06(k);
                break;
            case 7:
                result = my_tree.task07(k);
                break;
            default:
                result = 0;
                break;
        }
        clock_t end_time = clock();
        double elapsed_time = ((double) (end_time - start_time)) / CLOCKS_PER_SEC;
        printf("%s : Task = %d k = %d Result = %d Elapsed = %.2f\n",
               argv[0], task, k, result, elapsed_time);
    }

    printf("New tree:\n");
    my_tree.print(p);

    return 0;
}
