#include <cstdio>
#include <ctime>
#include <iostream>

#include "io.h"
#include "tree.h"

int main(int argc, char *argv[]) {
    int r = 0, task = 4;
    const char *filename = nullptr;

    if (!(argc == 3 && sscanf(argv[1], "%d", &r) == 1)) {
        printf("Usage %s : r filename\n", argv[0]);
        return 1;
    }

    if (r <= 0) {
        printf("Error: r must be positive\n");
        return 1;
    }
    filename = argv[2];

    tree my_tree;
    FILE *fp = fopen(filename, "r");
    if (fp == nullptr) {
        printf("Can not open file %s\n", filename);
        return -1;
    }

    io_status status = my_tree.read(fp);
    fclose(fp);

    if (status != io_status::success) {
        printf("Error reading file\n");
        return 1;
    }
    printf("Original tree:\n");
    my_tree.print(r);
    int T = my_tree.get_count_total();
    int L = my_tree.get_count_leaf();
    int C1 = my_tree.get_count_1();
    int C2 = my_tree.get_count_2();
    int H = my_tree.get_height();
    int W = my_tree.get_width();
    int B = my_tree.get_balance();
    printf("%s : Task = %d T = %d L = %d C1 = %d C2 = %d H = %d W = %d B = %d\n",
           argv[0], task, T, L, C1, C2, H, W, B);

    double t = clock();
    my_tree.task4_transform();
    t = (clock() - t) / CLOCKS_PER_SEC;

    printf("Modified tree:\n");
    my_tree.print(r);
    T = my_tree.get_count_total();
    L = my_tree.get_count_leaf();
    C1 = my_tree.get_count_1();
    C2 = my_tree.get_count_2();
    H = my_tree.get_height();
    W = my_tree.get_width();
    B = my_tree.get_balance();

    printf(
            "%s : Task = %d T = %d L = %d C1 = %d C2 = %d H = %d W = %d B = %d Elapsed = %.2f\n",
            argv[0], task, T, L, C1, C2, H, W, B, t);

    return 0;
}