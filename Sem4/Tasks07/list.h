#ifndef LIST_H
#define LIST_H

#include "record.h"
#include "command.h"
#include "hash_index.h"

class list_node : public record {
private:
    list_node *next = nullptr;
    list_node *prev = nullptr;

public:
    list_node *name_hash_next = nullptr;
    list_node *phone_hash_next = nullptr;

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

    void delete_list();

    void remove_node(list_node *node);

public:
    list2(int k_name, int k_phone) : name_index(k_name), phone_index(k_phone) {
    }

    ~list2() { delete_list(); }

    list2(const list2 &) = delete;

    list2 &operator=(const list2 &) = delete;

    io_status read_from_file(const char *filename);

    bool insert(const record &r);

    int remove(const command &cmd);

    int select_and_print(const command &cmd, FILE *fp = stdout) const;

    void print_stats() const;
};

#endif
