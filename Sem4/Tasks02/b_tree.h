#ifndef B_TREE_H
#define B_TREE_H

#include <stdio.h>
#include <string.h>
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
        values = nullptr;
        children = nullptr;
        size = 0;
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
    b_tree(int i = 0) : m(i) {
    }

    b_tree(const b_tree &) = delete;

    b_tree &operator=(const b_tree &) = delete;

    b_tree(b_tree &&x) : m(x.m), root(x.root) {
        x.root = nullptr;
        x.m = 0;
    }

    b_tree &operator=(b_tree &&x) {
        if (this == &x) return *this;
        delete_subtree(root);
        m = x.m;
        root = x.root;
        x.root = nullptr;
        x.m = 0;
        return *this;
    }

    ~b_tree() {
        delete_subtree(root);
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

    bool contains(const char *word) const {
        return contains_rec(root, word);
    }

    void print(int r, FILE *fp = stdout) const {
        print_subtree(root, 0, r, fp);
    }

    io_status read(FILE *fp = stdin, unsigned int max_read = -1) {
        unsigned int readed = 0;
        io_status ret;
        T x;
        while ((readed < max_read) && (x.read(fp) == io_status::success)) {
            if ((ret = add_value(x)) != io_status::success) {
                delete_subtree(root);
                root = nullptr;
                return io_status::memory;
            }
            readed++;
        }
        if ((!feof(fp)) && (readed < max_read)) {
            delete_subtree(root);
            root = nullptr;
            return io_status::format;
        }
        return io_status::success;
    }

    io_status read_file(const char *filename, unsigned int max_read = -1) {
        FILE *fp = fopen(filename, "r");
        if (!fp)
            return io_status::open;
        io_status ret = read(fp, max_read);
        fclose(fp);
        return ret;
    }

private:
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

    bool contains_rec(const b_tree_node<T> *node, const char *word) const {
        if (!node) return false;
        int i = 0;
        while (i < node->size && strcmp(word, node->values[i].get_name()) > 0) {
            i++;
        }
        if (i < node->size && strcmp(word, node->values[i].get_name()) == 0) {
            return true;
        }
        if (node->children[i]) {
            return contains_rec(node->children[i], word);
        }
        return false;
    }
};

#endif
