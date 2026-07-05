#ifndef LIST_H
#define LIST_H

#include "record.h"
#include "command.h"
#include <stdio.h>

class list_node : public record {
private:
    list_node *next = nullptr;
    list_node *prev = nullptr;

public:
    list_node() = default;

    ~list_node() = default;

    list_node(const list_node &) = delete;

    list_node(list_node &&r) noexcept: record((record &&) r) {
        next = r.next;
        r.next = nullptr;
        prev = r.prev;
        r.prev = nullptr;
    }

    list_node &operator=(const list_node &) = delete;

    list_node &operator=(list_node &&r) noexcept {
        if (this != &r) {
            *(record *) this = (record &&) r;
            next = r.next;
            r.next = nullptr;
            prev = r.prev;
            r.prev = nullptr;
        }
        return *this;
    }

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

public:
    list2() = default;

    ~list2() { delete_list(); }

    list2(const list2 &) = delete;

    list2 &operator=(const list2 &) = delete;

    list2(list2 &&x) noexcept: head(x.head), tail(x.tail) {
        x.head = nullptr;
        x.tail = nullptr;
    }

    list2 &operator=(list2 &&x) noexcept {
        if (this != &x) {
            delete_list();
            head = x.head;
            tail = x.tail;
            x.head = nullptr;
            x.tail = nullptr;
        }
        return *this;
    }

    io_status read_from_file(const char *filename);

    int apply_and_print(const command &cmd, FILE *fp = stdout) const;

    void delete_list();
};

#endif