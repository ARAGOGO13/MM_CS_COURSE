#include "list.h"
#include <cstdio>
#include <new>

void list2::delete_list() {
    list_node *cur = head;
    while (cur) {
        list_node *next = cur->get_next();
        delete cur;
        cur = next;
    }
    head = tail = nullptr;
}

io_status list2::read_from_file(const char *filename) {
    FILE *fp = fopen(filename, "r");
    if (!fp) return io_status::open;

    delete_list();

    while (true) {
        list_node *n = new(std::nothrow) list_node;
        if (!n) {
            delete_list();
            fclose(fp);
            return io_status::memory;
        }

        io_status st = n->read(fp);
        if (st == io_status::eof) {
            delete n;
            break;
        }
        if (st != io_status::success) {
            delete n;
            delete_list();
            fclose(fp);
            return st;
        }

        n->set_prev(tail);
        n->set_next(nullptr);
        if (tail) tail->set_next(n);
        tail = n;
        if (!head) head = n;
    }

    fclose(fp);
    return io_status::success;
}

int list2::apply_and_print(const command &cmd, FILE *fp) const {
    int found = 0;
    list_node *cur = head;
    while (cur) {
        if (cmd.apply(*cur)) {
            cur->print(cmd.get_order(), fp);
            ++found;
        }
        cur = cur->get_next();
    }
    fprintf(fp, "\n");
    return found;
}