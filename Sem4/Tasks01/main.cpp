#include <cstdio>
#include <ctime>
#include <cstdlib>
#include "student.h"
#include "avl_tree.h"

int main(int argc, char *argv[]) {
    int r = 0;
    const char *filename = nullptr;
    const char *s = nullptr;

    if (!(argc == 4 && sscanf(argv[1], "%d", &r) == 1)) {
        printf("Usage: %s r s filename\n", argv[0]);
        return 1;
    }

    if (r <= 0) {
        printf("Error: r must be positive\n");
        return 1;
    }

    s = argv[2];
    filename = argv[3];

    avl_tree<student> *tree = new avl_tree<student>();

    io_status status = tree->read_from_file(filename);
    if (status != io_status::success) {
        switch (status) {
            case io_status::open:
                printf("Cannot open %s\n", filename);
                break;
            case io_status::format:
                printf("Cannot read %s\n", filename);
                break;
            case io_status::memory:
                printf("Not enough memory\n");
                break;
            default:
                printf("Read error\n");
                break;
        }
        delete tree;
        return 1;
    }

    tree->print(r);

    for (int task = 1; task <= 5; ++task) {
        const char *s_all = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";

        clock_t t0_all = clock();
        int res_all = 0;
        switch (task) {
            case 1: res_all = tree->task01(s_all);
                break;
            case 2: res_all = tree->task02(s_all);
                break;
            case 3: res_all = tree->task03(s_all);
                break;
            case 4: res_all = tree->task04(s_all);
                break;
            case 5: res_all = tree->task05(s_all);
                break;
            default: res_all = 0;
                break;
        }
        clock_t t1_all = clock();
        double t_all = double(t1_all - t0_all) / CLOCKS_PER_SEC;

        clock_t t0 = clock();
        int res = 0;
        switch (task) {
            case 1: res = tree->task01(s);
                break;
            case 2: res = tree->task02(s);
                break;
            case 3: res = tree->task03(s);
                break;
            case 4: res = tree->task04(s);
                break;
            case 5: res = tree->task05(s);
                break;
            default: res = 0;
                break;
        }
        clock_t t1 = clock();
        double t = double(t1 - t0) / CLOCKS_PER_SEC;

        printf("%s : Task = %d S = %s Result = %d Elapsed = %.2f\n",
               argv[0], task, s_all, res_all, t_all);
        printf("%s : Task = %d S = %s Result = %d Elapsed = %.2f\n",
               argv[0], task, s, res, t);
    }

    delete tree;
    return 0;
}
