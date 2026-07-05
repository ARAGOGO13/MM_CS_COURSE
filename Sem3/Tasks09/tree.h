#ifndef TREE_H
#define TREE_H

#include "student.h"
#include "io.h"
#include <cstdio>

class tree;

class tree_node : public student {
private:
    tree_node *down = nullptr;
    tree_node *level = nullptr;

    void erase_links() {
        down = nullptr;
        level = nullptr;
    }

public:
    tree_node() = default;

    tree_node(const tree_node &x) = delete;

    tree_node(tree_node &&r) : student((student &&) r) {
        erase_links();
        r.erase_links();
    }

    ~tree_node() { erase_links(); }

    tree_node &operator=(const tree_node &) = delete;

    tree_node &operator=(tree_node &&r) {
        if (this == &r) return *this;
        (student &&) *this = (student &&) r;
        erase_links();
        r.erase_links();
        return *this;
    }

    friend class tree;
};

class tree {
private:
    tree_node *root = nullptr;

    static void delete_subtree(tree_node *curr);

    static void print_subtree(tree_node *curr, int level, int r);

    static void add_node(tree_node *curr, tree_node *x);

    int t1(tree_node *n, int k);

    int t2(tree_node *n, int k, int *sz_out);

    int t3(tree_node *n, int k, int *h_out, int *sz_out);

    int t4(tree_node *n, int k);

    int t6(tree_node *n, int k, int *h_out);

    int t7(tree_node **np, int k);

    bool check_lvls(tree_node *n, int k);

    int cnt_lvl(tree_node *n, int lvl);

    int cnt_all(tree_node *n);

    int cnt_leq(tree_node *n, int k);

    int delete_min(int k);

public:
    tree() = default;

    tree(const tree &x) = delete;

    tree(tree &&x) {
        root = x.root;
        x.root = nullptr;
    }

    ~tree() {
        delete_subtree(root);
        root = nullptr;
    }

    tree &operator=(const tree &x) = delete;

    tree &operator=(tree &&x) {
        if (this == &x) return *this;
        delete_subtree(root);
        root = x.root;
        x.root = nullptr;
        return *this;
    }

    void print(int r) const { print_subtree(root, 0, r); }

    io_status read(FILE *fp);

    int task01(int k);

    int task02(int k);

    int task03(int k);

    int task04(int k);

    int task05(int k);

    int task06(int k);

    int task07(int k);
};

io_status read_file(tree *tr, const char *filename);

#endif
