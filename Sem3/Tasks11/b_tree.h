#ifndef b_tree_H
#define b_tree_H

#include <stdio.h>
#include <string.h>
#include "student.h"
#include "io.h"

template<class T>
class b_tree_node;

template<class T>
class b_tree;

template<class T>
class b_tree_node {
    T *values = nullptr;
    int size = 0;
    b_tree_node **children = nullptr;

public:
    b_tree_node() = default;

    b_tree_node(const b_tree_node &x) = delete;

    b_tree_node(b_tree_node &&x) {
        values = x.values;
        children = x.children;
        size = x.size;
        x.erase_links();
    }

    b_tree_node &operator=(const b_tree_node &x) = delete;

    b_tree_node &operator=(b_tree_node &&x) {
        if (this == &x)
            return *this;
        delete_node();

        values = x.values;
        children = x.children;
        size = x.size;

        x.erase_links();
        return *this;
    }

    ~b_tree_node() {
        delete_node();
    }

    void print(int p, FILE *fp = stdout) const {
        for (int i = 0; i < size; ++i) {
            for (int j = 0; j < p; j++)
                fprintf(fp, " ");
            fprintf(fp, "values[%2d] ", i + 1);
            values[i].print(fp);
        }
    }

    int bin_search(const T &x) const {
        int l = 0, r = size, m;
        while (l != r) {
            m = (l + r) / 2;
            if (values[m] < x)
                l = m + 1;
            else
                r = m;
        }
        return l;
    }

    friend class b_tree<T>;

private:
    void erase_links() {
        values
                = nullptr;
        children = nullptr;
        size
                = 0;
    }

    void delete_node() {
        if (values != nullptr)
            delete[] values;
        if (children != nullptr)
            delete[] children;
        erase_links();
    }

    io_status init(int m) {
        values = new T[2 * m];
        if (values == nullptr)
            return io_status::memory;
        children = new b_tree_node *[2 * m + 1];
        if (children == nullptr) {
            delete[] values;
            values = nullptr;
            return io_status::memory;
        }
        for (int i = 0; i < 2 * m + 1; ++i)
            children[i] = nullptr;
        size = 0;
        return io_status::success;
    }


    void add_value(T &x, b_tree_node<T> *down, int index) {
        for (int i = size; i > index; i--) {
            values[i] = static_cast<T &&>(values[i - 1]);
            children[i + 1] = children[i];
        }
        values[index] = static_cast<T &&>(x);
        children[index + 1] = down;
        size++;
    }
};

template<class T>
class b_tree {
private:
    int m = 0;
    b_tree_node<T> *root = nullptr;

public:
    b_tree(int i = 0) { m = i; }

    io_status read(FILE *fp = stdin, unsigned int max_read = -1) {
        unsigned int readed = 0;
        io_status ret;
        T x;

        while ((readed < max_read) && (x.read(fp) == io_status::success)) {
            if ((ret = add_value(x)) != io_status::success) {
                delete_subtree(root);
                erase_links();

                return io_status::memory;
            }
            readed++;
        }
        if ((!feof(fp)) && (readed < max_read)) {
            delete_subtree(root);
            erase_links();

            return io_status::format;
        }

        return io_status::success;
    }

    io_status read_file(char *filename, unsigned int max_read = -1) {
        FILE *fp = fopen(filename, "r");
        if (!fp)
            return io_status::open;

        io_status ret = read(fp, max_read);

        fclose(fp);
        return ret;
    }

    void print(int r, FILE *fp = stdout) const {
        print_subtree(root, 0, r, fp);
    }

    ~b_tree() {
        delete_subtree(root);
        erase_links();
    }

    int task01(int k) { return task01_rec(root, k); }

    int task02(int k) {
        int total_nodes = 0;
        int total_elements = 0;
        return task02_rec(root, k, &total_nodes, &total_elements);
    }

    int task03(int k) {
        int total_nodes = 0;
        int height = 0;
        return task03_rec(root, k, &total_nodes, &height);
    }

    int task04(int k) { return task04_rec(root, k); }

    int task05(int k) { return count_total_at_level(root, k); }

    int task06(int k) {
        if (k <= 0 || !root) return 0;
        int depth = 0;
        while (count_nodes_at_level(root, depth) > 0) {
            ++depth;
        }
        if (depth != k) return 0;
        return count_total_in_subtree(root);
    }

private:
    void erase_links() {
        m = 0;
        root = nullptr;
    }

    static void delete_subtree(b_tree_node<T> *curr) {
        if (curr == nullptr)
            return;
        for (int i = 0; i <= curr->size; i++)
            delete_subtree(curr->children[i]);
        delete curr;
    }

    static void print_subtree(b_tree_node<T> *curr,
                              int level, int r, FILE *fp = stdout) {
        if (curr == nullptr || level > r)
            return;
        curr->print(level, fp);
        for (int i = 0; i <= curr->size; i++) {
            if (curr->children[i] && level + 1 <= r) {
                for (int j = 0; j < level; j++)
                    fprintf(fp, " ");
                fprintf(fp, "children[%2d]\n", i);
                print_subtree(curr->children[i], level + 1, r, fp);
            }
        }
    }

    io_status add_value(T &x) {
        if (root == nullptr) {
            root = new b_tree_node<T>();
            if (root == nullptr)
                return io_status::memory;
            if (root->init(m) != io_status::success) {
                delete root;
                return io_status::memory;
            }
            root->values[0] = static_cast<T &&>(x);
            root->size = 1;
            return io_status::success;
        }
        b_tree_node<T> *curr = root, *down = nullptr;
        io_status r = add_value_subtree(curr, down, x, m);
        if (r == io_status::memory)
            return io_status::memory;
        if (r == io_status::success)
            return io_status::success;
        b_tree_node<T> *p = new b_tree_node<T>();
        if (p == nullptr)
            return io_status::memory;
        if (p->init(m) != io_status::success) {
            delete p;
            return io_status::memory;
        }
        p->values[0] = static_cast<T &&>(x);
        p->children[0] = curr;
        p->children[1] = down;
        p->size = 1;
        root = p;
        return io_status::success;
    }

    static io_status add_value_subtree
            (b_tree_node<T> *&curr, b_tree_node<T> *&down, T &x, int m) {
        int index = curr->bin_search(x);
        b_tree_node<T> *new_node = curr->children[index];
        if (new_node != nullptr) {
            io_status r = add_value_subtree(new_node, down, x, m);
            if (r == io_status::memory)
                return io_status::memory;
            if (r == io_status::success)
                return io_status::success;
        } else
            down = nullptr;
        if (curr->size < 2 * m) {
            curr->add_value(x, down, index);
            return io_status::success;
        } else {
            b_tree_node<T> *p = new b_tree_node<T>();
            if (p == nullptr)
                return io_status::memory;
            if (p->init(m) != io_status::success) {
                delete p;
                return io_status::memory;
            }
            if (index == m) {
                for (int i = 1; i <= m; i++) {
                    p->values[i - 1]
                            = static_cast<T &&>(curr->values[i + m - 1]);
                    p->children[i] = curr->children[i + m];
                    curr->children[i + m] = nullptr;
                }
                p->children[0] = down;
            }
            if (index < m) {
                for (int i = 0; i < m; i++) {
                    p->values[i]
                            = static_cast<T &&>(curr->values[i + m]);
                    p->children[i] = curr->children[i + m];
                    curr->children[i + m] = nullptr;
                }
                p->children[m] = curr->children[2 * m];
                curr->children[2 * m] = nullptr;
                for (int i = m; i > index; i--) {
                    curr->values[i]
                            = static_cast<T &&>(curr->values[i - 1]);
                    curr->children[i + 1] = curr->children[i];
                }
                curr->children[index + 1] = down;
                curr->values[index] = static_cast<T &&>(x);
                x = static_cast<T &&>(curr->values[m]);
            }
            if (index > m) {
                p->children[0] = curr->children[m + 1];
                curr->children[m + 1] = nullptr;
                for (int i = 1; i < index - m; i++) {
                    p->values[i - 1]
                            = static_cast<T &&>(curr->values[i + m]);
                    p->children[i] = curr->children[i + m + 1];
                    curr->children[i + m + 1] = nullptr;
                }
                p->values[index - m - 1] = static_cast<T &&>(x);
                p->children[index - m] = down;
                for (int i = index - m + 1; i <= m; i++) {
                    p->values[i - 1]
                            = static_cast<T &&>(curr->values[i + m - 1]);
                    p->children[i] = curr->children[i + m];
                    curr->children[i + m] = nullptr;
                }
                x = static_cast<T &&>(curr->values[m]);
            }
            down = p;
            p->size = m;
            curr->size = m;
            return io_status::create;
        }
        return io_status::success;
    }

    int task01_rec(b_tree_node<T> *&curr, int k) {
        if (curr == nullptr) return 0;

        int cnt = 0;
        int child_count = 0;

        for (int i = 0; i <= curr->size; i++) {
            if (curr->children[i] == nullptr) break;
            child_count++;
            cnt += task01_rec(curr->children[i], k);
        }
        return cnt + (child_count == k ? curr->size : 0);
    }

    int task02_rec(b_tree_node<T> *&curr, int k, int *out_total_nodes,
                   int *out_total_elements) {
        if (curr == nullptr) {
            *out_total_nodes = 0;
            *out_total_elements = 0;
            return 0;
        }

        int result = 0;
        int total_nodes = 1;
        int total_elements = curr->size;

        for (int i = 0; i <= curr->size; i++) {
            if (curr->children[i] == nullptr) break;
            int child_nodes = 0;
            int child_elements = 0;
            int child_result = task02_rec(curr->children[i], k, &child_nodes,
                                          &child_elements);
            result += child_result;
            total_nodes += child_nodes;
            total_elements += child_elements;
        }

        if (total_nodes <= k) {
            result = total_elements;
        }

        *out_total_nodes = total_nodes;
        *out_total_elements = total_elements;
        return result;
    }

    int task03_rec(b_tree_node<T> *&curr, int k, int *out_total_elements,
                   int *out_height) {
        if (curr == nullptr) {
            *out_total_elements = 0;
            *out_height = 0;
            return 0;
        }

        int result = 0;
        int max_height = 0;
        int total_elements = curr->size;

        for (int i = 0; i <= curr->size; i++) {
            if (curr->children[i] == nullptr) break;
            int child_elements = 0;
            int child_height = 0;
            int child_result = task03_rec(curr->children[i], k, &child_elements,
                                          &child_height);
            result += child_result;
            if (child_height > max_height) {
                max_height = child_height;
            }
        }

        if (max_height < k) {
            result += total_elements;
        }

        *out_total_elements = total_elements;
        *out_height = max_height + 1;
        return result;
    }

    int task04_rec(b_tree_node<T> *&curr, int k) {
        if (curr == nullptr) return 0;
        if (check_subtree_levels_less_k(curr, k)) {
            return count_total_in_subtree(curr);
        }

        int result = 0;
        for (int i = 0; i <= curr->size; i++) {
            if (!curr->children[i]) break;
            result += task04_rec(curr->children[i], k);
        }
        return result;
    }

    int count_nodes_at_level(b_tree_node<T> *&curr, int level) {
        if (curr == nullptr) return 0;
        if (level == 0) return 1;
        int total = 0;
        for (int i = 0; i <= curr->size; i++) {
            if (!curr->children[i]) break;
            total += count_nodes_at_level(curr->children[i], level - 1);
        }
        return total;
    }

    int count_total_in_subtree(b_tree_node<T> *&curr) {
        if (curr == nullptr) return 0;
        int total = curr->size;

        for (int i = 0; i <= curr->size; i++) {
            if (!curr->children[i]) break;
            total += count_total_in_subtree(curr->children[i]);
        }
        return total;
    }

    bool check_subtree_levels_less_k(b_tree_node<T> *&curr, int k) {
        if (curr == nullptr) return true;
        int level = 0;
        int count = 0;
        while ((count = count_nodes_at_level(curr, level)) > 0) {
            if (count > k) return false;
            level++;
        }
        return true;
    }

    int count_total_at_level(b_tree_node<T> *&curr, int level) {
        if (curr == nullptr) return 0;
        if (level == 0) return curr->size;

        int total = 0;
        for (int i = 0; i <= curr->size; i++) {
            if (!curr->children[i]) break;
            total += count_total_at_level(curr->children[i], level - 1);
        }
        return total;
    }
};

#endif