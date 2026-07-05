#ifndef TREE_H
#define TREE_H

#include "student.h"
#include "io.h"

class tree;

class tree_node : public student {
private:
    tree_node *left = nullptr;
    tree_node *right = nullptr;

public:
    tree_node() = default;

    tree_node(const tree_node &x) = delete;

    tree_node(tree_node &&x) : student((student &&) x) {
        erase_links();
        x.erase_links();
    }

    ~tree_node() {
        erase_links();
    }

    tree_node &operator=(const tree_node &x) = delete;

    tree_node &operator=(tree_node &&x) {
        if (this == &x)
            return *this;
        (student &&) *this = (student &&) x;
        erase_links();
        x.erase_links();
        return *this;
    }

    friend class tree;

private:
    void erase_links() {
        left = nullptr;
        right = nullptr;
    }
};

class tree {
private:
    tree_node *root = nullptr;

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
        if (this == &x)
            return *this;
        delete_subtree(root);
        root = x.root;
        x.root = nullptr;
        return *this;
    }

    void print(unsigned int r = 10, FILE *fp = stdout) const {
        print_subtree(root, 0, r, fp);
    }

    io_status read(FILE *fp = stdin, unsigned int max_read = (unsigned int) -1) {
        if (fp == nullptr) return io_status::format;
        student temp;
        unsigned int cnt = 0;
        while (true) {
            io_status st = temp.read(fp);
            if (st == io_status::success) {
                tree_node *new_node = new tree_node();
                if (new_node == nullptr) {
                    return io_status::memory;
                }
                (student &) *new_node = (student &&) temp;
                add_node_subtree(root, new_node);
                cnt++;
                if (max_read != (unsigned int) -1 && cnt >= max_read) {
                    return io_status::success;
                }
                continue;
            } else if (st == io_status::eof) {
                return io_status::success;
            } else {
                return st;
            }
        }
    }

    int get_count_total() const { return count_total_nodes(root); }

    int get_count_leaf() const { return count_leaf_nodes(root); }

    int get_count_1() const { return count_single_child_nodes(root); }

    int get_count_2() const { return count_two_child_nodes(root); }

    int get_height() const { return height_of_tree(root); }

    int get_width() const { return width_of_tree(root); }

    int get_balance() const { return max_balance(root); }

    void make_max();

    void sort_levels_by_value_min();

    void task3_transform();

    void task4_transform();

    void task5_transform(const student &x);

    void task6_transform(const student &x);

    void task7_transform(const student &x);


private:
    static void print_subtree(tree_node *curr, int level, int r, FILE *fp) {
        if (curr == nullptr || level > r) return;
        for (int i = 0; i < level * 2; i++) fprintf(fp, " ");
        curr->print(fp);
        fprintf(fp, "\n");
        print_subtree(curr->left, level + 1, r, fp);
        print_subtree(curr->right, level + 1, r, fp);
    }

    void add_node_subtree(tree_node *&curr, tree_node *x) {
        if (curr == nullptr) {
            curr = x;
            return;
        }
        if (*x < *curr) add_node_subtree(curr->left, x); else add_node_subtree(curr->right, x);
    }

    static void delete_subtree(tree_node *curr) {
        if (curr == nullptr) return;
        delete_subtree(curr->left);
        delete_subtree(curr->right);
        delete curr;
    }

    static int count_leaf_nodes(const tree_node *node) {
        if (node == nullptr) return 0;
        if (node->left == nullptr && node->right == nullptr) return 1;
        return count_leaf_nodes(node->left) + count_leaf_nodes(node->right);
    }

    static int count_total_nodes(tree_node *node) {
        if (node == nullptr) return 0;
        return 1 + count_total_nodes(node->left) + count_total_nodes(node->right);
    }

    static int count_single_child_nodes(const tree_node *node) {
        if (node == nullptr) return 0;
        int count = 0;
        if ((node->left == nullptr && node->right != nullptr) || (node->left != nullptr && node->right == nullptr))
            count = 1;
        return count + count_single_child_nodes(node->left) + count_single_child_nodes(node->right);
    }

    static int count_two_child_nodes(const tree_node *node) {
        if (node == nullptr) return 0;
        int count = 0;
        if (node->left != nullptr && node->right != nullptr) count = 1;
        return count + count_two_child_nodes(node->left) + count_two_child_nodes(node->right);
    }

    static int height_of_tree(const tree_node *node) {
        if (node == nullptr) return 0;
        int left_len = height_of_tree(node->left);
        int right_len = height_of_tree(node->right);
        return 1 + (left_len > right_len ? left_len : right_len);
    }

    static int count_nodes_at_level(const tree_node *node, int level) {
        if (node == nullptr || level <= 0) return 0;
        if (level == 1) return 1;
        return count_nodes_at_level(node->left, level - 1) + count_nodes_at_level(node->right, level - 1);
    }

    static int width_of_tree(const tree_node *node) {
        if (node == nullptr) return 0;
        int h = height_of_tree(node);
        int maxc = 0;
        for (int level = 1; level <= h; ++level) {
            int c = count_nodes_at_level(node, level);
            if (c > maxc) maxc = c;
        }
        return maxc;
    }

    static int height_and_update_balance(const tree_node *node, int &max_balance_out) {
        if (node == nullptr) return 0;
        int left_h = height_and_update_balance(node->left, max_balance_out);
        int right_h = height_and_update_balance(node->right, max_balance_out);
        int diff = left_h - right_h;
        if (diff < 0) diff = -diff;
        if (diff > max_balance_out) max_balance_out = diff;
        return 1 + (left_h > right_h ? left_h : right_h);
    }

    static int max_balance(const tree_node *node) {
        int mb = 0;
        height_and_update_balance(node, mb);
        return mb;
    }

    static void swap_students(tree_node *a, tree_node *b);

    static void heapify_subtree(tree_node *node);

    static tree_node *node_at_level_index(tree_node *node, int level, int &index);

    void find_target_and_min_at_level(tree_node *node, int level, int target_pos, int &pos_counter, tree_node *&target_node,
                                      tree_node *&min_node);

    void task3_transform_sub(tree_node *node, tree_node *parent);

    void task4_transform_sub(tree_node *node, tree_node *parent);

    int task5_rec(tree_node *&root_node, const char *s, int k);

    int task6_rec(tree_node *&root_node, const char *s, int k);

    int longest_match_depth_from_node(tree_node* curr_root, const char* pattern, int &flag);

    void remove_subtree_with_deep_match(tree_node* parent, int is_lr, tree_node* curr_root, const char* pattern, int k);

    int count_nodes_and_longest_run_on_level(const char* pattern, int ur, int curr_ur, tree_node* curr_root, int &c, int &max_c);

    void remove_subtrees_starting_at_level(tree_node* curr_root, int ur, int curr_ur);



        bool find_rightmost_at_level(tree_node *node, int level, const char *s, int k, int &last_start, int &last_len);

    void find_rightmost(tree_node *node, int level, const char *s, int k, int &pos, int &curr_start, int &curr_len,
                        int &last_start, int &last_len);

    void delete_nodes_at_level(tree_node *&curr, int level, int start, int len, int &pos);
};
#endif
