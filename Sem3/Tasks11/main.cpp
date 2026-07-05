#include <cstdio>
#include <ctime>
#include <cstdlib>
#include "student.h"
#include "b_tree.h"

int main(int argc, char* argv[]) {
  int m = 0, r = 0, k = 0;
  char* filename = nullptr;

  if (!(argc == 5 && sscanf(argv[1], "%d", &m) == 1 && sscanf(argv[2], "%d", &r)
        == 1 && sscanf(argv[3], "%d", &k) == 1)) {
    printf("Usage: %s m r k filename\n", argv[0]);
    return 1;
  }

  if (r <= 0 || m <= 0) {
    printf("Error: r and m must be positive\n");
    return 1;
  }
  filename = argv[4];

  b_tree<student>* a = new b_tree<student>(m);

  io_status status = a->read_file(filename);
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
      case io_status::open:
        printf("Cannot open\n");
        break;
      case io_status::create:
        printf("Can not create\n");
        break;
      default:
        printf("Read error\n");
        break;
    }
    delete a;
    return 1;
  }

  printf("Initial tree:\n");
  a->print(r);

  for (int task = 1; task <= 6; ++task) {
    clock_t start_time = clock();
    int result = 0;
    switch (task) {
      case 1:
        result = a->task01(k);
        break;
      case 2:
        result = a->task02(k);
        break;
      case 3:
        result = a->task03(k);
        break;
      case 4:
        result = a->task04(k);
        break;
      case 5:
        result = a->task05(k);
        break;
      case 6:
        result = a->task06(k);
        break;
      default:
        result = 0;
        break;
    }
    clock_t end_time = clock();
    double elapsed_time = ((double)(end_time - start_time)) / CLOCKS_PER_SEC;
    printf("%s : Task = %d M = %d K = %d Result = %d Elapsed = %.2f\n",
           argv[0], task, m, k, result, elapsed_time);
  }

  delete a;
  return 0;
}