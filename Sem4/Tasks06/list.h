#ifndef LIST_H
#define LIST_H

#include "record.h"
#include "command.h"
#include "hash_index.h"
#include <stdio.h>

class list_node : public record {
private:
    list_node *next = nullptr;
    list_node *prev = nullptr;

public:
    list_node *hash_next = nullptr;

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
    hash_index index;

    void delete_list();

    void remove_node(list_node *node);

public:
    list2(int k) : index(k) {
    }

    ~list2() { delete_list(); }

    list2(const list2 &) = delete;

    list2 &operator=(const list2 &) = delete;

    io_status read_from_file(const char *filename);

    bool insert(const record &r);

    int remove(const command &cmd);

    int select_and_print(const command &cmd, FILE *fp = stdout) const;
};

#endif
