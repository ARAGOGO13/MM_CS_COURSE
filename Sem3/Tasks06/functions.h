#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#include "io.h"
#include <climits>

class list {
private:
  list_node* head = nullptr;

  list_node* split_segment(list_node* start, int size);

  list_node* merge_two(list_node* a, list_node* b, list_node*& out_tail);

  void append_node(list_node*& h, list_node*& t, list_node* node);

  list_node* quick_sort_rec(list_node* start, int len, list_node*& out_tail);

public:
  list() = default;

  ~list();

  void set_head(list_node* h);

  list_node* get_head() const;

  void clear();

  int get_less_than_previous() const;

  void sort_bubble();

  void sort_selection();

  void sort_insertion();

  void sort_merge();

  void sort_quick();

  io_status read(FILE* fp, unsigned int max_read = UINT_MAX);

  void print(unsigned int r, FILE* fp = stdout) const;
};

#endif