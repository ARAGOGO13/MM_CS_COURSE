#ifndef AVL_TREE_H
#define AVL_TREE_H

#include <stdio.h>
#include <string.h>
#include "io.h"

template<class T>
class avl_tree;

template<class T>
class avl_tree_node : public T {
private:
    avl_tree_node *left = nullptr;
    avl_tree_node *right = nullptr;
    int balance = 0;

public:
    avl_tree_node() = default;

    avl_tree_node(const avl_tree_node &x) = delete;

    avl_tree_node(avl_tree_node &&r) : T((T &&) r) {
        left = r.left;
        right = r.right;
        balance = r.balance;
        r.erase_links();
    }

    avl_tree_node(T &&x) : T(std::move(x)) {
        left = right = nullptr;
        balance = 0;
    }

    ~avl_tree_node() { erase_links(); }

    avl_tree_node &operator=(const avl_tree_node &) = delete;

    avl_tree_node &operator=(avl_tree_node &&r) {
        if (this == &r) return *this;
        (T &&) *this = (T &&) r;
        left = r.left;
        right = r.right;
        balance = r.balance;
        r.erase_links();
        return *this;
    }

    friend class avl_tree<T>;

private:
    void erase_links() {
        left = nullptr;
        right = nullptr;
        balance = 0;
    }
};

template<class T>
class avl_tree {
private:
    avl_tree_node<T> *root = nullptr;

public:
    avl_tree() = default;

    avl_tree(const avl_tree &x) = delete;

    avl_tree(avl_tree &&x) {
        root = x.root;
        x.root = nullptr;
    }

    ~avl_tree() {
        delete_subtree(root);
        root = nullptr;
    }

    avl_tree &operator=(const avl_tree &x) = delete;

    avl_tree &operator=(avl_tree &&x) {
        if (this == &x) return *this;
        delete_subtree(root);
        root = x.root;
        x.root = nullptr;
        return *this;
    }

    void insert(T &&x) {
        avl_tree_node<T> *node = new avl_tree_node<T>(std::move(x));
        avl_insert(node);
    }

    bool contains(const char *word) const {
        return contains_rec(root, word);
    }

    const T *find(const char *word) const {
        return find_rec(root, word);
    }

    void print(int r, FILE *fp = stdout) const {
        print_sub(root, 0, r, fp);
    }

    io_status read(FILE *fp = stdin, unsigned int max_nodes = (unsigned int) -1) {
        unsigned int read_cnt = 0;
        avl_tree_node<T> temp;
        while (read_cnt < max_nodes) {
            io_status st = temp.read(fp);
            if (st == io_status::eof) break;
            if (st != io_status::success) {
                delete_subtree(root);
                root = nullptr;
                return st;
            }
            avl_tree_node<T> *node = new(std::nothrow) avl_tree_node<T>((avl_tree_node<T> &&) temp);
            if (!node) {
                delete_subtree(root);
                root = nullptr;
                return io_status::memory;
            }
            avl_insert(node);
            ++read_cnt;
        }
        return io_status::success;
    }

private:
    avl_tree_node<T> *rotate_right(avl_tree_node<T> *y) {
        avl_tree_node<T> *x = y->left;
        y->left = x->right;
        x->right = y;
        return x;
    }

    avl_tree_node<T> *rotate_left(avl_tree_node<T> *x) {
        avl_tree_node<T> *y = x->right;
        x->right = y->left;
        y->left = x;
        return y;
    }

    avl_tree_node<T> *rotate_left_right(avl_tree_node<T> *node) {
        node->left = rotate_left(node->left);
        return rotate_right(node);
    }

    avl_tree_node<T> *rotate_right_left(avl_tree_node<T> *node) {
        node->right = rotate_right(node->right);
        return rotate_left(node);
    }

    void avl_insert(avl_tree_node<T> *z) {
        bool grew = false;
        root = avl_insert_rec(root, z, grew);
    }

    avl_tree_node<T> *avl_insert_rec(avl_tree_node<T> *node, avl_tree_node<T> *to_insert, bool &grew) {
        if (!node) {
            to_insert->left = to_insert->right = nullptr;
            to_insert->balance = 0;
            grew = true;
            return to_insert;
        }

        if (*to_insert < *node) {
            node->left = avl_insert_rec(node->left, to_insert, grew);
            if (grew) {
                if (node->balance == 1) {
                    node->balance = 0;
                    grew = false;
                } else if (node->balance == 0) {
                    node->balance = -1;
                    grew = true;
                } else {
                    if (node->left && node->left->balance == -1) {
                        node = rotate_right(node);
                        node->balance = 0;
                        if (node->right) node->right->balance = 0;
                    } else {
                        avl_tree_node<T> *L = node->left;
                        avl_tree_node<T> *R = L ? L->right : nullptr;
                        int rbal = R ? R->balance : 0;
                        node = rotate_left_right(node);
                        if (rbal == -1) {
                            if (node->left) node->left->balance = 0;
                            if (node->right) node->right->balance = 1;
                        } else if (rbal == 0) {
                            if (node->left) node->left->balance = 0;
                            if (node->right) node->right->balance = 0;
                        } else {
                            if (node->left) node->left->balance = -1;
                            if (node->right) node->right->balance = 0;
                        }
                        node->balance = 0;
                    }
                    grew = false;
                }
            }
        } else {
            node->right = avl_insert_rec(node->right, to_insert, grew);
            if (grew) {
                if (node->balance == -1) {
                    node->balance = 0;
                    grew = false;
                } else if (node->balance == 0) {
                    node->balance = 1;
                    grew = true;
                } else {
                    if (node->right && node->right->balance == 1) {
                        node = rotate_left(node);
                        node->balance = 0;
                        if (node->left) node->left->balance = 0;
                    } else {
                        avl_tree_node<T> *R = node->right;
                        avl_tree_node<T> *L = R ? R->left : nullptr;
                        int lbal = L ? L->balance : 0;
                        node = rotate_right_left(node);
                        if (lbal == 1) {
                            if (node->left) node->left->balance = -1;
                            if (node->right) node->right->balance = 0;
                        } else if (lbal == 0) {
                            if (node->left) node->left->balance = 0;
                            if (node->right) node->right->balance = 0;
                        } else {
                            if (node->left) node->left->balance = 0;
                            if (node->right) node->right->balance = 1;
                        }
                        node->balance = 0;
                    }
                    grew = false;
                }
            }
        }
        return node;
    }

    static void delete_subtree(avl_tree_node<T> *n) {
        if (!n) return;
        delete_subtree(n->left);
        delete_subtree(n->right);
        delete n;
    }

    static void print_sub(avl_tree_node<T> *n, int lvl, int max_lvl, FILE *fp = stdout) {
        if (!n) return;
        if (lvl > max_lvl) return;
        for (int i = 0; i < lvl; i++) fprintf(fp, "  ");
        n->print(fp);
        print_sub(n->left, lvl + 1, max_lvl, fp);
        print_sub(n->right, lvl + 1, max_lvl, fp);
    }

    bool contains_rec(const avl_tree_node<T> *node, const char *word) const {
        if (!node) return false;
        int cmp = strcmp(word, node->get_name());
        if (cmp == 0) return true;
        if (cmp < 0) return contains_rec(node->left, word);
        else return contains_rec(node->right, word);
    }

    const T *find_rec(const avl_tree_node<T> *node, const char *word) const {
        if (!node) return nullptr;
        int cmp = strcmp(word, node->get_name());
        if (cmp == 0) return node;
        if (cmp < 0) return find_rec(node->left, word);
        else return find_rec(node->right, word);
    }
};

#endif
