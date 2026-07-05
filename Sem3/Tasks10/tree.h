#ifndef tree_H
#define tree_H

#include <stdio.h>
#include <string.h>
#include "student.h"
#include "list.h"

template<class T>
class tree;

template<class T>
class tree_node : public T {
private:
    tree_node *left = nullptr;
    tree_node *right = nullptr;
public:
    tree_node() = default;

    tree_node(const tree_node &x) = delete;

    tree_node(tree_node &&r) : T((T &&) r) {
        erase_links();
        r.erase_links();
    }

    ~tree_node() { erase_links(); }

    tree_node &operator=(const tree_node &) = delete;

    tree_node &operator=(tree_node &&r) {
        if (this == &r) return *this;
        (T &&) *this = (T &&) r;
        erase_links();
        r.erase_links();
        return *this;
    }

    friend class tree<T>;

private:
    void erase_links() {
        left = nullptr;
        right = nullptr;
    }
};

template<class T>
class tree {
private:
    tree_node<T> *root = nullptr;
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

    static int sum_leaves(const tree_node<T> *n) {
        if (!n) return 0;
        if ((n->left == nullptr) && (n->right == nullptr))
            return n->get_length();
        return sum_leaves(n->left) + sum_leaves(n->right);
    }

    int task01() const {
        return sum_leaves(root);
    }

    static int max_branch(const tree_node<T> *n) {
        if (!n) return 0;
        int left_val = max_branch(n->left);
        int right_val = max_branch(n->right);
        int mx = (left_val > right_val) ? left_val : right_val;
        return mx + n->get_length();
    }

    int task02() const {
        return max_branch(root);
    }

    static int level_sum_at(const tree_node<T> *n, int lvl) {
        if (!n) return 0;
        if (lvl == 0) return n->get_length();
        return level_sum_at(n->left, lvl - 1) + level_sum_at(n->right, lvl - 1);
    }

    int level_max() const {
        if (!root) return 0;
        int mx = 0;
        int lvl = 0;
        for (;;) {
            int s = level_sum_at(root, lvl);
            if (s == 0) break;
            if (s > mx) mx = s;
            ++lvl;
        }
        return mx;
    }

    int task03() const {
        return level_max();
    }

    int compute_diff(const tree_node<T> *n, int &out_sum) const {
        if (!n) {
            out_sum = 0;
            return 0;
        }

        int left_sum = 0;
        int right_sum = 0;

        int left_mx = compute_diff(n->left, left_sum);
        int right_mx = compute_diff(n->right, right_sum);

        out_sum = left_sum + right_sum + n->get_length();

        int diff = left_sum - right_sum;
        if (diff < 0) diff = -diff;

        int mx = left_mx;
        if (right_mx > mx) mx = right_mx;
        if (diff > mx) mx = diff;

        return mx;
    }

    int task04() const {
        if (!root) return 0;
        int total = 0;
        return compute_diff(root, total);
    }


    static int sum_one_child(const tree_node<T> *n) {
        if (!n) return 0;
        int acc = 0;
        bool l = (n->left != nullptr);
        bool r = (n->right != nullptr);
        if (l ^ r) acc += n->get_length();
        acc += sum_one_child(n->left);
        acc += sum_one_child(n->right);
        return acc;
    }

    int task05() const {
        return sum_one_child(root);
    }

    int task06() {
        if (!root) return 0;
        int m = find_min();
        return delete_min(m);
    }

    int find_min() {
        int cur = root->get_value();
        int result = cur;
        if (root->left) {
            tree<T> L;
            L.root = root->left;
            int lm = L.find_min();
            if (lm < result) result = lm;
            L.root = nullptr;
        }
        if (root->right) {
            tree<T> R;
            R.root = root->right;
            int rm = R.find_min();
            if (rm < result) result = rm;
            R.root = nullptr;
        }
        return result;
    }

    int delete_min(int target) {
        if (!root) return 0;
        if (root->get_value() == target) {
            delete_subtree(root);
            root = nullptr;
            return 1;
        }
        tree<T> L;
        L.root = root->left;
        tree<T> R;
        R.root = root->right;
        int cnt = 0;
        cnt += L.delete_min(target);
        cnt += R.delete_min(target);
        if (!L.root) root->left = nullptr;
        if (!R.root) root->right = nullptr;
        L.root = nullptr;
        R.root = nullptr;
        return cnt;
    }

    void print(unsigned int r = 10, FILE *fp = stdout) const {
        print_sub(root, 0, (int) r, fp);
    }

    io_status read(FILE *fp = stdin, unsigned int max_nodes = (unsigned int) -1) {
        unsigned int read_cnt = 0;
        tree_node<T> temp;
        while (read_cnt < max_nodes) {
            io_status st = temp.read(fp);
            if (st != io_status::success) {
                if (st == io_status::success) continue;
                if (st == io_status::eof) break;
                if (!feof(fp)) {
                    delete_subtree(root);
                    root = nullptr;
                    return st;
                }
                break;
            }
            tree_node<T> *node = new tree_node<T>((tree_node<T> &&) temp);
            if (!node) {
                delete_subtree(root);
                root = nullptr;
                return io_status::memory;
            }
            if (!root) root = node;
            else insert_node(root, node);
            ++read_cnt;
        }
        if (!feof(fp) && read_cnt >= max_nodes) {
            delete_subtree(root);
            root = nullptr;
            return io_status::format;
        }
        return io_status::success;
    }

private:
    static void delete_subtree(tree_node<T> *n) {
        if (!n) return;
        delete_subtree(n->left);
        delete_subtree(n->right);
        delete n;
    }

    static void insert_node(tree_node<T> *cur, tree_node<T> *node) {
        tree_node<T> *p = cur;
        for (;;) {
            if (*node < *p) {
                if (!p->left) {
                    p->left = node;
                    return;
                }
                p = p->left;
            } else {
                if (!p->right) {
                    p->right = node;
                    return;
                }
                p = p->right;
            }
        }
    }

    static void print_sub(tree_node<T> *n, int lvl, int max_lvl, FILE *fp = stdout) {
        if (!n) return;
        if (lvl > max_lvl) return;
        int sp = 2 * lvl;
        n->print(fp, sp);
        print_sub(n->left, lvl + 1, max_lvl, fp);
        print_sub(n->right, lvl + 1, max_lvl, fp);
    }

    static int level_sum(tree_node<T> *n, int level) {
        if (!n) return 0;
        if (level == 0) return n->get_length();
        int a = level_sum(n->left, level - 1);
        int b = level_sum(n->right, level - 1);
        return a + b;
    }
};


template<class T>
io_status read_from_file(tree<T> *tr, const char *name) {
    if (!tr) return io_status::format;
    FILE *f = fopen(name, "r");
    if (!f) return io_status::eof;
    io_status res = tr->read(f);
    fclose(f);
    return res;
}

#endif
