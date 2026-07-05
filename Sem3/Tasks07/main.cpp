#include <cstdio>
#include <ctime>
#include <iostream>

#include "io.h"

int main(int argc, char* argv[]) {
  int r = 0;
  const char* filename = nullptr;

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
  FILE* fp = fopen(filename, "r");
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

  printf("Tree:\n");
  my_tree.print(r);

  for (int task_num = 1; task_num <= 5; task_num++) {
    clock_t start_time = clock();
    int result = 0;

    switch (task_num) {
      case 1:
        result = my_tree.task01();
        break;
      case 2:
        result = my_tree.task02();
        break;
      case 3:
        result = my_tree.task03();
        break;
      case 4:
        result = my_tree.task04();
        break;
      case 5:
        result = my_tree.task05();
        break;
    }

    clock_t end_time = clock();
    double elapsed_time = ((double)(end_time - start_time)) / CLOCKS_PER_SEC;

    printf("%s : Task = %d Result = %d Elapsed = %.2f\n",
           argv[0], task_num, result, elapsed_time);
  }

  clock_t start_time = clock();
  int result6 = my_tree.task06();
  clock_t end_time = clock();
  double elapsed_time6 = ((double)(end_time - start_time)) / CLOCKS_PER_SEC;

  printf("%s : Task = %d Result = %d Elapsed = %.2f\n",
         argv[0], 6, result6, elapsed_time6);

  printf("New tree:\n");
  my_tree.print(r);

  return 0;
}