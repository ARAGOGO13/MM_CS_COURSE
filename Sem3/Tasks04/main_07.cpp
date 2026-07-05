#include <cstdio>
#include <ctime>
#include <iostream>

#include "list_io.h"
#include "functions.h"
#include "io.h"

using namespace std;

int main(int argc, char* argv[]) {
  int r = 0;
  const char* filename = nullptr;

  if (!(argc == 3 && sscanf(argv[1], "%d", &r) == 1)) {
    printf("Usage: %s n [file]\n", argv[0]);
    return 1;
  }

  if (r <= 0) {
    printf("Error: r must be positive\n");
    return 1;
  }
  filename = argv[2];

  io_status status;
  list_node* head = nullptr;
  FILE* fp = fopen(filename, "r");
  if (fp == nullptr)
    {
      printf("Can not open file %s\n", filename);
      return -1;
    }
  status = read_list(&head, fp);

  if (status != io_status::success) {
    switch (status) {
      case io_status::eof:
        printf("Error: empty file or end of file\n");
        break;
      case io_status::format:
        printf("Error: invalid file format\n");
        break;
      case io_status::memory:
        printf("Error: memory allocation failed\n");
        break;
      default:
        printf("Error: cannot read file '%s'\n", filename);
        break;
    }
    return 1;
  }

  if (head == nullptr) {
    printf("Error: no data read from file\n");
    return 1;
  }

  print_list(head, r);

  clock_t start_time = clock();
  int result = max_distance_between_constant_segments(head);
  clock_t end_time = clock();

  double elapsed_time = ((double)(end_time - start_time)) / CLOCKS_PER_SEC;

  printf("%s : Task = %d Result = %d Elapsed = %.2f\n",
         argv[0], 7, result, elapsed_time);

  delete_list(head);

  return 0;
}
