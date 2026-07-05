#include <cstdio>
#include <ctime>
#include <cstdlib>
#include "student.h"
#include "list.h"
#include "tree.h"

int list2::m = 0;
int list2::r = 0;


int main(int argc, char *argv[]) {
    int r = 0;
    int m = 0;
    const char *filename = nullptr;

    if (!(argc == 4 && sscanf(argv[1], "%d", &r) == 1 && sscanf(argv[3], "%d", &m) == 1)) {
        printf("Usage: %s r filename m\n", argv[0]);
        return 1;
    }

    if (r <= 0) {
        printf("Error: r must be positive\n");
        return 1;
    }
    filename = argv[2];

    tree<student> student_tree;
    io_status status = read_from_file(&student_tree, filename);
    if (status != io_status::success) {
        switch (status) {
            case io_status::eof:
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
        return 1;
    }

    printf("Initial tree:\n");
    student_tree.print(r);

    for (int task = 1; task <= 6; ++task) {
        clock_t start_time = clock();
        int result = 0;
        switch (task) {
            case 1:
                result = student_tree.task01();
                break;
            case 2:
                result = student_tree.task02();
                break;
            case 3:
                result = student_tree.task03();
                break;
            case 4:
                result = student_tree.task04();
                break;
            case 5:
                result = student_tree.task05();
                break;
            case 6:
                result = student_tree.task06();
                break;
            default:
                result = 0;
                break;
        }
        clock_t end_time = clock();
        double elapsed_time = ((double) (end_time - start_time)) / CLOCKS_PER_SEC;
        printf("%s : Task = %d M = %d Result = %d Elapsed = %.2f\n",
               argv[0], task, m, result, elapsed_time);
    }

    printf("New tree:\n");
    student_tree.print(r);

    list2::set_m(m);
    list2::set_r(r);

    tree<list2> list_tree;
    status = read_from_file(&list_tree, filename);
    if (status != io_status::success) {
        switch (status) {
            case io_status::eof:
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
        return 1;
    }

    printf("\nInitial tree:\n");
    list_tree.print(r);

    for (int task = 1; task <= 6; ++task) {
        clock_t start_time = clock();
        int result = 0;
        switch (task) {
            case 1:
                result = list_tree.task01();
                break;
            case 2:
                result = list_tree.task02();
                break;
            case 3:
                result = list_tree.task03();
                break;
            case 4:
                result = list_tree.task04();
                break;
            case 5:
                result = list_tree.task05();
                break;
            case 6:
                result = list_tree.task06();
                break;
            default:
                result = 0;
                break;
        }
        clock_t end_time = clock();
        double elapsed_time = ((double) (end_time - start_time)) / CLOCKS_PER_SEC;
        printf("%s : Task = %d M = %d Result = %d Elapsed = %.2f\n",
               argv[0], task, m, result, elapsed_time);
    }

    printf("New tree:\n");
    list_tree.print(r);

    return 0;
}



