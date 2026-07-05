#include "list.h"
#include <stdio.h>

list2::~list2() {
    delete_list();
    head = nullptr;
}

list2 &list2::operator=(list2 &&x) {
    if (this == &x) return *this;
    this->delete_list();
    head = x.head;
    x.head = nullptr;
    return *this;
}

io_status list2::read(FILE *fp) {
    unsigned int count = 0;
    unsigned int max_read = m;
    list2_node temp;
    list2_node *curr;
    list2_node *new_node;
    io_status status;

    if (max_read == 0) return io_status::success;

    count++;
    if ((status = temp.read(fp)) != io_status::success) return io_status::format;

    head = new list2_node;
    if (head == nullptr) return io_status::memory;
    *head = (list2_node &&) temp;
    curr = head;

    while ((count < max_read) && (temp.read(fp) == io_status::success)) {
        new_node = new list2_node;
        if (new_node == nullptr) {
            this->delete_list();
            return io_status::memory;
        }
        *new_node = (list2_node &&) temp;
        curr->set_next(new_node);
        new_node->set_prev(curr);
        curr = new_node;
        count++;
    }

    if ((!feof(fp)) && (count < max_read)) {
        this->delete_list();
        return io_status::format;
    }
    return io_status::success;
}

void list2::print(FILE *fp, int spaces) const {
    int counter = 0;
    const list2_node *curr = head;

    while (curr != nullptr) {
        if (counter >= r) break;

        int j = 0;
        while (j < spaces) {
            printf(" ");
            j++;
        }

        curr->print(fp);
        curr = curr->get_next();
        counter++;
    }
}

int list2::get_length() const {
    int count = 0;
    list2_node *curr = head;

    while (curr != nullptr) {
        count++;
        curr = curr->get_next();
    }
    return count;
}

int list2::get_value() const {
    if (head == nullptr) return 0;

    int min_val = head->get_value();
    list2_node *curr = head->get_next();

    while (curr != nullptr) {
        if (curr->get_value() < min_val) {
            min_val = curr->get_value();
        }
        curr = curr->get_next();
    }
    return min_val;
}

int list2::operator<(const list2 &b) const {
    if ((head == nullptr) && (b.head == nullptr)) return 1;
    if (head == nullptr) return 0;
    if (b.head == nullptr) return 0;
    if (*head < *(b.head)) return 1;
    return 0;
}

void list2::delete_list() {
    list2_node *curr = head;
    list2_node *next_node;

    while (curr != nullptr) {
        next_node = curr->get_next();
        delete curr;
        curr = next_node;
    }
    head = nullptr;
}