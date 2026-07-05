#ifndef IO_H
#define IO_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <algorithm>

enum class io_status {
  success,
  eof,
  format,
  memory,
};

class student {
private:
  char* name = nullptr;
  int value = 0;

public:
  student() = default;

  student(const student& x) { init(x.name, x.value); }

  student(student&& x) {
    name = x.name;
    x.name = nullptr;
    value = x.value;
    x.value = 0;
  }

  ~student() { erase(); }

  student& operator=(const student& x) {
    if (this == &x) return *this;
    erase();
    init(x.name, x.value);
    return *this;
  }

  student& operator=(student&& x) {
    if (this == &x) return *this;
    erase();
    name = x.name;
    x.name = nullptr;
    value = x.value;
    x.value = 0;
    return *this;
  }

  void print(FILE* fp = stdout) const {
    if (name != nullptr)
      fprintf(fp, "%s %d\n", name, value);
    else
      fprintf(fp, "null %d\n", value);
  }

  io_status read(FILE* fp = stdin) {
    const int LEN = 1234;
    char n[LEN];
    int v;

    if (fscanf(fp, "%s%d", n, &v) != 2) {
      if (!feof(fp)) return io_status::format;
      return io_status::eof;
    }
    erase();
    return init(n, v);
  }

  int operator>(const student& x) const { return cmp(x) > 0; }
  int operator>=(const student& x) const { return cmp(x) >= 0; }
  int operator<(const student& x) const { return cmp(x) < 0; }
  int operator<=(const student& x) const { return cmp(x) <= 0; }
  int operator==(const student& x) const { return cmp(x) == 0; }
  int operator!=(const student& x) const { return cmp(x) != 0; }

private:
  io_status init(const char* n, int v) {
    value = v;
    if (n != nullptr) {
      size_t len = strlen(n);
      name = new char[len + 1];
      if (name != nullptr) {
        strcpy(name, n);
      } else {
        return io_status::memory;
      }
    }
    return io_status::success;
  }

  void erase() {
    if (name != nullptr) {
      delete[] name;
      name = nullptr;
    }
    value = 0;
  }

  int cmp(const student& x) const {
    if (name == nullptr && x.name == nullptr) return value - x.value;
    if (name == nullptr) return -1;
    if (x.name == nullptr) return 1;
    int res = strcmp(name, x.name);
    return res != 0 ? res : value - x.value;
  }
};

class list2_node : public student {
private:
  list2_node* next = nullptr;
  list2_node* prev = nullptr;

public:
  list2_node() = default;

  list2_node(const list2_node& r) : student(r) { next = prev = nullptr; }

  list2_node(list2_node&& r) : student(std::move(r)) {
    next = r.next;
    prev = r.prev;
    r.next = r.prev = nullptr;
  }

  ~list2_node() { next = prev = nullptr; }

  list2_node* get_next() const { return next; }
  list2_node* get_prev() const { return prev; }
  void set_next(list2_node* n) { next = n; }
  void set_prev(list2_node* p) { prev = p; }
};

class list2 {
private:
  list2_node* head = nullptr;
  list2_node* tail = nullptr;
  unsigned int length = 0;

public:
  list2() = default;

  ~list2() { clear(); }

  void clear();

  io_status read(FILE* fp = stdin, unsigned int max_read = -1);

  void print(unsigned int r = 10, FILE* fp = stdout) const;

  unsigned int get_length() const { return length; }

  void shift_right(int k);

  void remove_greater_than_previous_k(int k);

  void remove_greater_than_next_k(int k);

  void remove_k_local_maxima(int k);

  void remove_constant_segments_longer_than_k(int k);

  void remove_non_increasing_segments_longer_than_k(int k);

  void remove_between_constant_segments_longer_than_k(int k);

  void remove_between_non_decreasing_segments_longer_than_k(int k);

private:
  void add_node(list2_node* node);

  void remove_node(list2_node* node);
};

#endif