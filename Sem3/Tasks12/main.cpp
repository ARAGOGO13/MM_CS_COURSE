#include <cstdio>
#include <ctime>
#include <cstdlib>
#include "student.h"
#include "rb_tree.h"

int main(int argc, char* argv[]) {
  int r = 0, k = 0;
  const char* filename = nullptr;

  if (!(argc == 4 && sscanf(argv[1], "%d", &r) == 1 &&
        sscanf(argv[2], "%d", &k) == 1)) {
    printf("Usage: %s r k filename\n", argv[0]);
    return 1;
  }

  if (r <= 0) {
    printf("Error: r must be positive\n");
    return 1;
  }

  filename = argv[3];

  rb_tree<student>* tree = new rb_tree<student>();

  io_status status = read_from_file(tree, filename);
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

  printf("Initial tree:\n");
  tree->print(r);

  for (int task = 1; task <= 5; ++task) {
    clock_t start_time = clock();
    int result = 0;

    switch (task) {
      case 1:
        result = tree->task01(k);
        break;
      case 2:
        result = tree->task02(k);
        break;
      case 3:
        result = tree->task03(k);
        break;
      case 4:
        result = tree->task04(k);
        break;
      case 5:
        result = tree->task05(k);
        break;
      default:
        result = 0;
        break;
    }

    clock_t end_time = clock();
    double elapsed_time = ((double)(end_time - start_time)) / CLOCKS_PER_SEC;

    printf("%s : Task = %d K = %d Result = %d Elapsed = %.2f\n",
           argv[0], task, k, result, elapsed_time);
  }

  delete tree;
  return 0;
}