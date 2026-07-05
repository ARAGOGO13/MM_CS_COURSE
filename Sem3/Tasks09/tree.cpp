#include "tree.h"
#include <cstddef>
#include <cstdio>

void tree::delete_subtree(tree_node *curr) {
    if (curr == nullptr) return;
    tree_node *p = curr->down;
    tree_node *next = nullptr;
    for (; p; p = next) {
        next = p->level;
        delete_subtree(p);
    }
    delete curr;
}

void tree::print_subtree(tree_node *curr, int level, int r) {
    if (curr == nullptr || level > r) return;
    int spaces = level * 2;
    for (int i = 0; i < spaces; ++i) putchar(' ');
    curr->print();
    putchar('\n');
    for (tree_node *p = curr->down; p; p = p->level)
        print_subtree(p, level + 1, r);
}

void tree::add_node(tree_node *curr, tree_node *x) {
    if (curr->down == nullptr) {
        curr->down = x;
        return;
    }

    if (*x < *curr) {
        if (*curr->down < *curr)
            add_node(curr->down, x);
        else {
            x->level = curr->down;
            curr->down = x;
        }
    } else if (*x == *curr) {
        if (curr->down->level != nullptr) {
            x->level = curr->down->level;
            curr->down->level = x;
        } else if (*curr->down < *curr) {
            curr->down->level = x;
        } else {
            x->level = curr->down;
            curr->down = x;
        }
    } else {
        tree_node *p;
        for (p = curr->down; p->level; p = p->level);
        if (*p > *curr)
            add_node(p, x);
        else
            p->level = x;
    }
}

io_status tree::read(FILE *fp) {
    if (!fp) return io_status::format;
    tree_node *node = new tree_node();
    io_status st = node->read(fp);
    if (st == io_status::success) {
        if (root == nullptr) root = node;
        else add_node(root, node);
        return io_status::success;
    } else {
        delete node;
        return st;
    }
}


io_status read_file(tree *tr, const char *filename) {
    if (!tr || !filename) return io_status::format;
    FILE *fp = fopen(filename, "r");
    if (!fp) return io_status::eof;
    while (true) {
        io_status st = tr->read(fp);
        if (st == io_status::success) continue;
        if (feof(fp)) {
            fclose(fp);
            return io_status::success;
        } else {
            fclose(fp);
            return st;
        }
    }
}
