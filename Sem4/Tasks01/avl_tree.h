#ifndef AVL_TREE_H
#define AVL_TREE_H

#include <stdio.h>
#include <string.h>
#include <new>
#include <stdlib.h>
#include "student.h"
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

    int task01(const char *s) const {
        if (!root) return 0;
        return task01_rec(root, s);
    }

    int task02(const char *s) const {
        if (!root) return 0;
        return task02_rec(root, s);
    }

    int task03(const char *s) const {
        if (!root) return 0;
        bool out_check = false;
        return task03_rec(root, s, &out_check);
    }

    int task04(const char *s) const {
        if (!root || !s) return 0;
        int h = tree_height(root);
        if (h <= 0) return 0;
        int *levels = new(std::nothrow) int[h];
        if (!levels) return 0;
        for (int i = 0; i < h; ++i) levels[i] = 0;
        task04_fill_levels(root, 0, h, levels, s);
        int max_count = 0;
        for (int i = 0; i < h; ++i) if (levels[i] > max_count) max_count = levels[i];
        delete[] levels;
        return max_count;
    }

    int task05(const char *s) const {
        if (!root || !s) return 0;
        int max_diff = 0;
        task05_rec(root, s, &max_diff);
        return max_diff;
    }

    void print(int r, FILE *fp = stdout) const {
        print_sub(root, 0, r, fp);
    }

    io_status read(FILE *fp = stdin, unsigned int max_nodes = (unsigned int) -1) {
        if (!fp) return io_status::open;
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
            avl_tree_node<T> *node = new avl_tree_node<T>((avl_tree_node<T> &&) temp);
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

    io_status read_from_file(const char *name) {
        if (!name) return io_status::format;
        FILE *f = fopen(name, "r");
        if (!f) return io_status::open;
        io_status res = read(f);
        fclose(f);
        return res;
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

    int name_consists_of_s_chars(const char *name, const char *s) const {
        if (!name || !s) return 0;
        for (const char *p = name; *p; ++p) {
            if (strchr(s, *p) == nullptr) return 0;
        }
        return 1;
    }

    int task01_rec(const avl_tree_node<T> *node, const char *s) const {
        if (!node) return 0;
        if (node->left == nullptr && node->right == nullptr) {
            const char *name = node->get_name();
            return name_consists_of_s_chars(name, s);
        }
        return task01_rec(node->left, s) + task01_rec(node->right, s);
    }

    int task02_rec(const avl_tree_node<T> *node, const char *s) const {
        if (!node) return 0;
        int mx_l = task02_rec(node->left, s);
        int mx_r = task02_rec(node->right, s);
        const char *name = node->get_name();
        return name_consists_of_s_chars(name, s) + ((mx_l > mx_r) ? mx_l : mx_r);
    }

    int task03_rec(const avl_tree_node<T> *node, const char *s, bool *out_check) const {
        if (!node) {
            if (out_check) *out_check = true;
            return 0;
        }
        bool left_check = false;
        bool right_check = false;
        int left_cnt = task03_rec(node->left, s, &left_check);
        int right_cnt = task03_rec(node->right, s, &right_check);
        const char *name = node->get_name();
        bool flag = name_consists_of_s_chars(name, s) && left_check && right_check;
        if (out_check) *out_check = flag;
        int total = left_cnt + right_cnt + (flag ? 1 : 0);
        return total;
    }

    int task05_rec(const avl_tree_node<T> *node, const char *s, int *out_mx_diff) const {
        if (!node) return 0;
        int left_cnt = task05_rec(node->left, s, out_mx_diff);
        int right_cnt = task05_rec(node->right, s, out_mx_diff);
        int diff = abs(left_cnt - right_cnt);
        if (diff > *out_mx_diff) *out_mx_diff = diff;
        const char *name = node->get_name();
        return left_cnt + right_cnt + name_consists_of_s_chars(name, s);
    }

    void task04_fill_levels(const avl_tree_node<T> *node, int level, int max_h, int *levels, const char *s) const {
        if (!node) return;
        if (level >= max_h) return;
        if (name_consists_of_s_chars(node->get_name(), s)) levels[level]++;
        if (node->left) task04_fill_levels(node->left, level + 1, max_h, levels, s);
        if (node->right) task04_fill_levels(node->right, level + 1, max_h, levels, s);
    }

    int tree_height(const avl_tree_node<T> *node) const {
        if (!node) return 0;
        int lh = tree_height(node->left);
        int rh = tree_height(node->right);
        return 1 + (lh > rh ? lh : rh);
    }

    int count_matches_at_level(const avl_tree_node<T> *node, int level, const char *s) const {
        if (!node) return 0;
        if (level == 0) {
            const char *name = node->get_name();
            return name_consists_of_s_chars(name, s);
        }
        return count_matches_at_level(node->left, level - 1, s)
               + count_matches_at_level(node->right, level - 1, s);
    }
};

#endif
