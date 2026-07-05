#ifndef LIST_H
#define LIST_H

#include "command.h"
#include "hash_index.h"
#include "record.h"

class list_node : public record {
 private:
  list_node *next = nullptr;
  list_node *prev = nullptr;

 public:
  list_node *name_hash_next = nullptr;
  list_node *phone_hash_next = nullptr;
  list_node *g_name_hash_next = nullptr;
  list_node *g_phone_hash_next = nullptr;
  list_node *group_next = nullptr;
  list_node *group_prev = nullptr;

  list_node *get_next() const { return next; }
  list_node *get_prev() const { return prev; }
  void set_next(list_node *n) { next = n; }
  void set_prev(list_node *p) { prev = p; }

  friend class list2;
};

class list2 {
 private:
  list_node *head = nullptr;
  list_node *tail = nullptr;
  hash_index<const char *> name_index;
  hash_index<int> phone_index;
  hash_index<const char *> *group_name_indexes[1000];
  hash_index<int> *group_phone_indexes[1000];
  list_node *group_heads[1000];
  list_node *group_tails[1000];

  void delete_list();
  void remove_node(list_node *node);

 public:
  list2(int k_name, int k_phone);
  ~list2();

  list2(const list2 &) = delete;
  list2 &operator=(const list2 &) = delete;

  io_status read_from_file(const char *filename);
  bool insert(const record &r);
  int remove(const command &cmd);
  int select_and_print(const command &cmd, FILE *fp = stdout) const;

  int select_and_send(const command &cmd, int fd) const;

  void print_stats() const;
};

#endif