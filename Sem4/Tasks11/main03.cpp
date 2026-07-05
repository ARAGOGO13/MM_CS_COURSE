#include "solve.h"
#include <stdio.h>
#include <time.h>

int main(int argc, char **argv) {
  if (!(argc == 3)) {
    printf("Usage %s filename_in filename_out\n", argv[0]);
    return -1;
  }
  const char *filename_in = argv[1];
  const char *filename_out = argv[2];
  int res = 0;
  int task = 3;

  double t = clock();
  switch (task03(filename_in, filename_out, res)) {
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