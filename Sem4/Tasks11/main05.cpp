#include "solve.h"
#include <stdio.h>
#include <time.h>

int main(int argc, char **argv) {
  if (!(argc == 5)) {
    printf("Usage %s filename_a filename_b filename_c t\n", argv[0]);
    return -1;
  }
  const char *filename_a = argv[1];
  const char *filename_b = argv[2];
  const char *filename_c = argv[3];
  std::string ts = argv[4];
  int res = 0;
  int task = 5;

  double t = clock();
  switch (task05(filename_a, filename_b, filename_c, ts, res)) {
  case io_status::open:
    printf("Couldn't open file\n");
    return -1;
  default:
    break;
  }
  t = (clock() - t) / CLOCKS_PER_SEC;

  printf("%s : Task = %d Result = %d Elapsed = %.2f\n", argv[0], task, res, t);
  return 0;
}