#ifndef list_H
#define list_H

#include "io.h"
#include "student.h"
#include <stdio.h>
#include <string.h>

class list2;

class list2_node : public student {
private:
    list2_node *next = nullptr;
    list2_node *prev = nullptr;
public:
    list2_node() = default;

    list2_node(const list2_node &) = delete;

    list2_node(list2_node &&r) : student((student &&) r) {
        next = r.next;
        r.next = nullptr;
        prev = r.prev;
        r.prev = nullptr;
    }

    ~list2_node() {
        next = nullptr;
        prev = nullptr;
    }

    list2_node &operator=(const list2_node &) = delete;

    list2_node &operator=(list2_node &&r) {
        *(student *) (this) = (student &&) r;
        next = r.next;
        r.next = nullptr;
        prev = r.prev;
        r.prev = nullptr;
        return *this;
    }

    list2_node *get_prev() const { return prev; }

    list2_node *get_next() const { return next; }

    void set_prev(list2_node *r) { prev = r; }

    void set_next(list2_node *r) { next = r; }

    list2_node *delete_node(int *flag, int i = 0);

    int is_max(int);

    friend class list2;
};

class list2 {
private:
    list2_node *head = nullptr;
    static int m;
    static int r;
public:
    list2() = default;

    list2(const list2 &x) = delete;

    list2(list2 &&x) {
        head = x.head;
        x.head = nullptr;
    }

    ~list2();

    list2 &operator=(const list2 &x) = delete;

    list2 &operator=(list2 &&x);

    int operator<(const list2 &b) const;

    io_status read(FILE *fp = stdin);

    void print(FILE *fp = stdout, int spaces = 0) const;

    int get_length() const;

    int get_value() const;

    static void set_m(int m_val) { m = m_val; }

    static void set_r(int r_val) { r = r_val; }

    void delete_list();
};

#endif