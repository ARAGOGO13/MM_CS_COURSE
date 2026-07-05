#ifndef RB_TREE_H
#define RB_TREE_H

#include <stdio.h>
#include <string.h>
#include "io.h"

enum class colors {
    invalid,
    red,
    black
};

template<class T>
class rb_tree;

template<class T>
class rb_tree_node : public T {
private:
    rb_tree_node *left = nullptr;
    rb_tree_node *right = nullptr;
    rb_tree_node *parent = nullptr;
    colors color = colors::invalid;

public:
    rb_tree_node() = default;

    rb_tree_node(const rb_tree_node &x) = delete;

    rb_tree_node(rb_tree_node &&r) : T((T &&) r) {
        left = r.left;
        right = r.right;
        parent = r.parent;
        color = r.color;
        r.erase_links();
    }

    rb_tree_node(T &&x) : T(std::move(x)) {
        left = right = parent = nullptr;
        color = colors::invalid;
    }

    ~rb_tree_node() { erase_links(); }

    rb_tree_node &operator=(const rb_tree_node &) = delete;

    rb_tree_node &operator=(rb_tree_node &&r) {
        if (this == &r) return *this;
        (T &&) *this = (T &&) r;
        left = r.left;
        right = r.right;
        parent = r.parent;
        color = r.color;
        r.erase_links();
        return *this;
    }

    friend class rb_tree<T>;

private:
    void erase_links() {
        left = nullptr;
        right = nullptr;
        parent = nullptr;
        color = colors::invalid;
    }
};

template<class T>
class rb_tree {
private:
    rb_tree_node<T> *root = nullptr;

public:
    rb_tree() = default;

    rb_tree(const rb_tree &x) = delete;

    rb_tree(rb_tree &&x) {
        root = x.root;
        x.root = nullptr;
    }

    ~rb_tree() {
        delete_subtree(root);
        root = nullptr;
    }

    rb_tree &operator=(const rb_tree &x) = delete;

    rb_tree &operator=(rb_tree &&x) {
        if (this == &x) return *this;
        delete_subtree(root);
        root = x.root;
        x.root = nullptr;
        return *this;
    }

    void insert(T &&x) {
        rb_tree_node<T> *node = new rb_tree_node<T>(std::move(x));
        rb_insert(node);
    }

    bool contains(const char *word) const {
        return contains_rec(root, word);
    }

    void print(int r, FILE *fp = stdout) const {
        print_sub(root, 0, r, fp);
    }

    io_status read(FILE *fp = stdin, unsigned int max_nodes = (unsigned int) -1) {
        unsigned int read_cnt = 0;
        rb_tree_node<T> temp;
        while (read_cnt < max_nodes) {
            io_status st = temp.read(fp);
            if (st == io_status::eof) break;
            if (st != io_status::success) {
                delete_subtree(root);
                root = nullptr;
                return st;
            }
            rb_tree_node<T> *node = new rb_tree_node<T>((rb_tree_node<T> &&) temp);
            if (!node) {
                delete_subtree(root);
                root = nullptr;
                return io_status::memory;
            }
            rb_insert(node);
            ++read_cnt;
        }
        return io_status::success;
    }

private:
    void left_rotate(rb_tree_node<T> *x) {
        rb_tree_node<T> *y = x->right;
        x->right = y->left;
        if (y->left != nullptr) y->left->parent = x;
        y->parent = x->parent;
        if (x->parent == nullptr) root = y;
        else if (x == x->parent->left) x->parent->left = y;
        else x->parent->right = y;
        y->left = x;
        x->parent = y;
    }

    void right_rotate(rb_tree_node<T> *x) {
        rb_tree_node<T> *y = x->left;
        x->left = y->right;
        if (y->right != nullptr) y->right->parent = x;
        y->parent = x->parent;
        if (x->parent == nullptr) root = y;
        else if (x == x->parent->right) x->parent->right = y;
        else x->parent->left = y;
        y->right = x;
        x->parent = y;
    }

    void rb_insert(rb_tree_node<T> *z) {
        rb_tree_node<T> *y = nullptr;
        rb_tree_node<T> *x = root;
        while (x != nullptr) {
            y = x;
            if (*z < *x) x = x->left;
            else x = x->right;
        }
        z->parent = y;
        if (y == nullptr) root = z;
        else if (*z < *y) y->left = z;
        else y->right = z;
        z->left = nullptr;
        z->right = nullptr;
        z->color = colors::red;
        rb_insert_fixup(z);
    }

    void rb_insert_fixup(rb_tree_node<T> *z) {
        while (z->parent != nullptr && z->parent->color == colors::red) {
            if (z->parent == z->parent->parent->left) {
                rb_tree_node<T> *y = z->parent->parent->right;
                if (y != nullptr && y->color == colors::red) {
                    z->parent->color = colors::black;
                    y->color = colors::black;
                    z->parent->parent->color = colors::red;
                    z = z->parent->parent;
                } else {
                    if (z == z->parent->right) {
                        z = z->parent;
                        left_rotate(z);
                    }
                    z->parent->color = colors::black;
                    z->parent->parent->color = colors::red;
                    right_rotate(z->parent->parent);
                }
            } else {
                rb_tree_node<T> *y = z->parent->parent->left;
                if (y != nullptr && y->color == colors::red) {
                    z->parent->color = colors::black;
                    y->color = colors::black;
                    z->parent->parent->color = colors::red;
                    z = z->parent->parent;
                } else {
                    if (z == z->parent->left) {
                        z = z->parent;
                        right_rotate(z);
                    }
                    z->parent->color = colors::black;
                    z->parent->parent->color = colors::red;
                    left_rotate(z->parent->parent);
                }
            }
        }
        if (root) root->color = colors::black;
    }

    static void delete_subtree(rb_tree_node<T> *n) {
        if (!n) return;
        delete_subtree(n->left);
        delete_subtree(n->right);
        delete n;
    }

    static void print_sub(rb_tree_node<T> *n, int lvl, int max_lvl,
                          FILE *fp = stdout) {
        if (!n) return;
        if (lvl > max_lvl) return;
        for (int i = 0; i < lvl; i++) fprintf(fp, "  ");
        if (n->color == colors::red) fprintf(fp, "RED: ");
        else fprintf(fp, "BLACK: ");
        n->print(fp);
        print_sub(n->left, lvl + 1, max_lvl, fp);
        print_sub(n->right, lvl + 1, max_lvl, fp);
    }

    bool contains_rec(const rb_tree_node<T> *node, const char *word) const {
        if (!node) return false;
        int cmp = strcmp(word, node->get_name());
        if (cmp == 0) return true;
        if (cmp < 0) return contains_rec(node->left, word);
        else return contains_rec(node->right, word);
    }
};

#endif
