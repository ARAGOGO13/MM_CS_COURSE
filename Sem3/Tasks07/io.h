#ifndef IO_H
#define IO_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

enum class io_status {
  success,
  eof,
  format,
  memory,
};

class student {
private:
  char* name = nullptr;
  int value = 0;

public:
  student() = default;

  student(const student& x) = delete;

  student(student&& x) {
    name = x.name;
    x.name = nullptr;
    value = x.value;
    x.value = 0;
  }

  ~student() {
    erase();
  }

  student& operator=(const student& x) = delete;

  student& operator=(student&& x) {
    if (this == &x)
      return *this;
    erase();
    name = x.name;
    x.name = nullptr;
    value = x.value;
    x.value = 0;
    return *this;
  }

  void print(FILE* fp = stdout) const {
    if (name != nullptr)
      fprintf(fp, "%s %d", name, value);
    else
      fprintf(fp, "null %d", value);
  }

  io_status read(FILE* fp = stdin) {
    const int LEN = 1234;
    char n[LEN];
    int v;

    if (fscanf(fp, "%s %d", n, &v) != 2) {
      if (!feof(fp))
        return io_status::format;
      return io_status::eof;
    }
    erase();
    return init(n, v);
  }

  int operator>(const student& x) const { return (cmp(x) > 0 ? 1 : 0); }
  int operator<(const student& x) const { return (cmp(x) < 0 ? 1 : 0); }
  int operator==(const student& x) const { return (cmp(x) == 0 ? 1 : 0); }

  int get_value() const { return value; }

private:
  io_status init(const char* n, int v) {
    value = v;
    if (n != nullptr) {
      size_t len = strlen(n);
      name = new(std::nothrow) char[len + 1];
      if (name != nullptr) {
        for (size_t i = 0; i <= len; i++)
          name[i] = n[i];
      } else {
        return io_status::memory;
      }
    } else {
      name = nullptr;
    }
    return io_status::success;
  }

  void erase() {
    value = 0;
    if (name != nullptr) {
      delete[] name;
      name = nullptr;
    }
  }

  int cmp(const student& x) const {
    if (name == nullptr) {
      if (x.name != nullptr)
        return -1;
      return value - x.value;
    }
    if (x.name == nullptr)
      return 1;
    int res = strcmp(name, x.name);
    if (res)
      return res;
    return value - x.value;
  }
};

class tree;

class tree_node : public student {
private:
  tree_node* left = nullptr;
  tree_node* right = nullptr;

public:
  tree_node() = default;

  tree_node(const tree_node& x) = delete;

  tree_node(tree_node&& x) : student((student&&)x) {
    erase_links();
    x.erase_links();
  }

  ~tree_node() {
    erase_links();
  }

  tree_node& operator=(const tree_node& x) = delete;

  tree_node& operator=(tree_node&& x) {
    if (this == &x)
      return *this;
    (student&&)*this = (student&&)x;
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
  tree_node* root = nullptr;

public:
  tree() = default;

  tree(const tree& x) = delete;

  tree(tree&& x) {
    root = x.root;
    x.root = nullptr;
  }

  ~tree() {
    delete_subtree(root);
    root = nullptr;
  }

  tree& operator=(const tree& x) = delete;

  tree& operator=(tree&& x) {
    if (this == &x)
      return *this;
    delete_subtree(root);
    root = x.root;
    x.root = nullptr;
    return *this;
  }

  void print(unsigned int r = 10, FILE* fp = stdout) const {
    print_subtree(root, 0, r, fp);
  }

  io_status read(FILE* fp = stdin, unsigned int max_read = (unsigned int)-1) {
    if (fp == nullptr) return io_status::format;
    student temp;
    unsigned int cnt = 0;
    while (true) {
      io_status st = temp.read(fp);
      if (st == io_status::success) {
        tree_node* new_node = new tree_node();
        if (new_node == nullptr) {
          return io_status::memory;
        }
        (student&)*new_node = (student&&)temp;
        add_node_subtree(root, new_node);
        cnt++;
        if (max_read != (unsigned int)-1 && cnt >= max_read) {
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

  int task01() { return count_leaf_nodes(root); }
  int task02() { return max_branch_length(root); }
  int task03() { return max_level_width(root); }
  int task04() { return max_depth_difference(root); }
  int task05() { return count_single_child_nodes(root); }
  int task06() { return remove_min_value_nodes(); }

private:
  static void print_subtree(tree_node* curr, int level, int r, FILE* fp) {
    if (curr == nullptr || level > r) return;
    for (int i = 0; i < level * 2; i++) fprintf(fp, " ");
    curr->print(fp);
    fprintf(fp, "\n");
    print_subtree(curr->left, level + 1, r, fp);
    print_subtree(curr->right, level + 1, r, fp);
  }

  void add_node_subtree(tree_node*& curr, tree_node* x) {
    if (curr == nullptr) {
      curr = x;
      return;
    }
    if (*x < *curr) {
      add_node_subtree(curr->left, x);
    } else {
      add_node_subtree(curr->right, x);
    }
  }

  int count_leaf_nodes(tree_node* node) {
    if (node == nullptr) return 0;
    if (node->left == nullptr && node->right == nullptr) return 1;
    return count_leaf_nodes(node->left) + count_leaf_nodes(node->right);
  }

  int max_branch_length(tree_node* node) {
    if (node == nullptr) return 0;
    int left_len = max_branch_length(node->left);
    int right_len = max_branch_length(node->right);
    return 1 + (left_len > right_len ? left_len : right_len);
  }

  int count_nodes_at_level(tree_node* node, int level) {
    if (node == nullptr) return 0;
    if (level == 1) return 1;
    return count_nodes_at_level(node->left, level - 1) +
           count_nodes_at_level(node->right, level - 1);
  }

  int max_level_width(tree_node* node) {
    int h = max_branch_length(node);
    if (h == 0) return 0;
    int max_width = 0;
    for (int lvl = 1; lvl <= h; ++lvl) {
      int w = count_nodes_at_level(node, lvl);
      if (w > max_width) max_width = w;
    }
    return max_width;
  }

  int depth_and_update_maxdiff(tree_node* node, int& maxdiff) {
    if (node == nullptr) return 0;
    int dl = depth_and_update_maxdiff(node->left, maxdiff);
    int dr = depth_and_update_maxdiff(node->right, maxdiff);
    int diff = dl - dr;
    if (diff < 0) diff = -diff;
    if (diff > maxdiff) maxdiff = diff;
    return 1 + (dl > dr ? dl : dr);
  }

  int max_depth_difference(tree_node* node) {
    int maxdiff = 0;
    depth_and_update_maxdiff(node, maxdiff);
    return maxdiff;
  }

  int count_single_child_nodes(tree_node* node) {
    if (node == nullptr) return 0;
    int count = 0;
    if ((node->left == nullptr && node->right != nullptr) ||
        (node->left != nullptr && node->right == nullptr)) {
      count = 1;
    }
    return count + count_single_child_nodes(node->left) +
           count_single_child_nodes(node->right);
  }

  static void delete_subtree(tree_node* curr) {
    if (curr == nullptr) return;
    delete_subtree(curr->left);
    delete_subtree(curr->right);
    delete curr;
  }

  static bool find_min_value(tree_node* node, int& minVal, bool& initialized) {
    if (node == nullptr) return false;
    int v = node->get_value();
    if (!initialized) {
      minVal = v;
      initialized = true;
    } else { if (v < minVal) minVal = v; }
    find_min_value(node->left, minVal, initialized);
    find_min_value(node->right, minVal, initialized);
    return initialized;
  }

  static int remove_min_nodes(tree_node*& curr, int minVal) {
    if (curr == nullptr) return 0;
    if (curr->get_value() == minVal) {
      delete_subtree(curr);
      curr = nullptr;
      return 1;
    }
    int cnt = 0;
    cnt += remove_min_nodes(curr->left, minVal);
    cnt += remove_min_nodes(curr->right, minVal);
    return cnt;
  }

  int remove_min_value_nodes() {
    if (root == nullptr) return 0;
    int minVal = 0;
    bool initialized = false;
    if (!find_min_value(root, minVal, initialized)) { return 0; }
    int removed_roots = remove_min_nodes(root, minVal);
    return removed_roots;
  }
};

#endif