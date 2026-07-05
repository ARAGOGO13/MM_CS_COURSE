#include <cstdio>
#include <ctime>
#include <iostream>
#include "io.h"

int main(int argc, char* argv[]) {
  int r = 0, k = 0;
  const char* filename = nullptr;

  if (!(argc == 4 && sscanf(argv[1], "%d", &r) == 1 && sscanf(argv[3], "%d", &k) == 1)) {
    printf("Usage %s : r file_name k\n", argv[0]);
    return 1;
  }

  if (r <= 0) {
    printf("Error: r must be positive\n");
    return 1;
  }
  if (k <= 0) {
    printf("Usage %s : r file_name k\n", argv[0]);
    return 1;
  }
  filename = argv[2];

  list2 lst;
  FILE* fp = fopen(filename, "r");
  if (fp == nullptr) {
    printf("Can not open file %s\n", filename);
    return -1;
  }

  io_status status = lst.read(fp);
  fclose(fp);

  if (status != io_status::success) {
    printf("Error reading file\n");
    return 1;
  }

  if (lst.get_length() == 0) {
    printf("Error: no data read from file\n");
    return 1;
  }

  printf("List :\n");
  lst.print(r);

  unsigned int len1 = lst.get_length();

  clock_t start_time = clock();
  lst.remove_constant_segments_longer_than_k(k);
  clock_t end_time = clock();

  unsigned int len2 = lst.get_length();
  double elapsed_time = ((double)(end_time - start_time)) / CLOCKS_PER_SEC;

  printf("New list :\n");
  lst.print(r);

  printf("%s : Task = %d Len Old = %u Len New = %u Elapsed = %.2f\n",
         argv[0], 5, len1, len2, elapsed_time);

  return 0;
}