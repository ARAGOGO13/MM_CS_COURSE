#ifndef RB_TREE_H
#define RB_TREE_H

#include <stdio.h>
#include <string.h>
#include "student.h"
#include "io.h"

enum class colors {
  invalid,
  red,
  black
};

template <class T>
class rb_tree;

template <class T>
class rb_tree_node : public T {
private:
  rb_tree_node* left = nullptr;
  rb_tree_node* right = nullptr;
  rb_tree_node* parent = nullptr;
  colors color = colors::invalid;

public:
  rb_tree_node() = default;

  rb_tree_node(const rb_tree_node& x) = delete;

  rb_tree_node(rb_tree_node&& r) : T((T&&)r) {
    left = r.left;
    right = r.right;
    parent = r.parent;
    color = r.color;
    r.erase_links();
  }

  ~rb_tree_node() { erase_links(); }

  rb_tree_node& operator=(const rb_tree_node&) = delete;

  rb_tree_node& operator=(rb_tree_node&& r) {
    if (this == &r) return *this;
    (T&&)*this = (T&&)r;
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

template <class T>
class rb_tree {
private:
  rb_tree_node<T>* root = nullptr;

public:
  rb_tree() = default;

  rb_tree(const rb_tree& x) = delete;

  rb_tree(rb_tree&& x) {
    root = x.root;
    x.root = nullptr;
  }

  ~rb_tree() {
    delete_subtree(root);
    root = nullptr;
  }

  rb_tree& operator=(const rb_tree& x) = delete;

  rb_tree& operator=(rb_tree&& x) {
    if (this == &x) return *this;
    delete_subtree(root);
    root = x.root;
    x.root = nullptr;
    return *this;
  }

  int task01(int k) const {
    if (!root || k < 1)
      return 0;

    int tmp_elems = 0;
    int tmp_nodes = 0;
    return task01_rec(root, k, &tmp_elems, &tmp_nodes);
  }

  int task02(int k) {
    int height, total_elements;
    return task02_rec(root, k, &height, &total_elements);
  }

  int task03(int k) {
    return task03_rec(root, k);
  }

  int task04(int k) {
    return count_elements_at_level(root, k);
  }

  int task05(int k) const {
    if (k < 0) return 0;
    if (leftmost_black_count(root) > k) return 0;
    return count_branches_with_depth(root, k - 1);
  }

  void print(int r, FILE* fp = stdout) const {
    print_sub(root, 0, r, fp);
  }

  io_status read(FILE* fp = stdin, unsigned int max_nodes = (unsigned int)-1) {
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
      rb_tree_node<T>* node = new rb_tree_node<T>((rb_tree_node<T>&&)temp);
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
  void left_rotate(rb_tree_node<T>* x) {
    rb_tree_node<T>* y = x->right;
    x->right = y->left;
    if (y->left != nullptr) y->left->parent = x;
    y->parent = x->parent;
    if (x->parent == nullptr) root = y;
    else if (x == x->parent->left) x->parent->left = y;
    else x->parent->right = y;
    y->left = x;
    x->parent = y;
  }

  void right_rotate(rb_tree_node<T>* x) {
    rb_tree_node<T>* y = x->left;
    x->left = y->right;
    if (y->right != nullptr) y->right->parent = x;
    y->parent = x->parent;
    if (x->parent == nullptr) root = y;
    else if (x == x->parent->right) x->parent->right = y;
    else x->parent->left = y;
    y->right = x;
    x->parent = y;
  }

  void rb_insert(rb_tree_node<T>* z) {
    rb_tree_node<T>* y = nullptr;
    rb_tree_node<T>* x = root;
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

  void rb_insert_fixup(rb_tree_node<T>* z) {
    while (z->parent != nullptr && z->parent->color == colors::red) {
      if (z->parent == z->parent->parent->left) {
        rb_tree_node<T>* y = z->parent->parent->right;
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
        rb_tree_node<T>* y = z->parent->parent->left;
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

  static void delete_subtree(rb_tree_node<T>* n) {
    if (!n) return;
    delete_subtree(n->left);
    delete_subtree(n->right);
    delete n;
  }

  static void print_sub(rb_tree_node<T>* n, int lvl, int max_lvl,
                        FILE* fp = stdout) {
    if (!n) return;
    if (lvl > max_lvl) return;
    for (int i = 0; i < lvl; i++) fprintf(fp, "  ");
    if (n->color == colors::red) fprintf(fp, "RED: ");
    else fprintf(fp, "BLACK: ");
    n->print(fp);
    print_sub(n->left, lvl + 1, max_lvl, fp);
    print_sub(n->right, lvl + 1, max_lvl, fp);
  }

  int task01_rec(const rb_tree_node<T>* node, int k,
                 int* out_total_elements, int* out_total_nodes) const {
    if (!node) {
      *out_total_elements = 0;
      *out_total_nodes = 0;
      return 0;
    }

    int left_elems = 0, left_nodes = 0;
    int right_elems = 0, right_nodes = 0;

    task01_rec(node->left, k, &left_elems, &left_nodes);
    task01_rec(node->right, k, &right_elems, &right_nodes);

    int total_nodes_in_subtree = 1 + left_nodes + right_nodes;

    int effective_nodes = total_nodes_in_subtree - (total_nodes_in_subtree > k);

    *out_total_nodes = effective_nodes;
    *out_total_elements = effective_nodes;

    return effective_nodes;
  }

  int task02_rec(rb_tree_node<T>* node, int k, int* out_height,
                 int* out_total_elements) {
    if (!node) {
      *out_height = 0;
      *out_total_elements = 0;
      return 0;
    }

    int res = 0;
    int total_elements = node->get_length();
    int max_child_height = 0;

    if (node->left) {
      int child_height, child_elements;
      int child_res = task02_rec(node->left, k, &child_height, &child_elements);
      res += child_res;
      total_elements += child_elements;
      if (child_height > max_child_height) max_child_height = child_height;
    }

    if (node->right) {
      int child_height, child_elements;
      int child_res =
          task02_rec(node->right, k, &child_height, &child_elements);
      res += child_res;
      total_elements += child_elements;
      if (child_height > max_child_height) max_child_height = child_height;
    }

    int current_height = max_child_height + 1;
    if (current_height <= k) {
      res = total_elements;
    }

    *out_height = current_height;
    *out_total_elements = total_elements;
    return res;
  }

  int task03_rec(rb_tree_node<T>* node, int k) {
    if (!node) return 0;
    if (check_subtree_levels_less_k(node, k)) {
      return count_total_in_subtree(node);
    }
    int res = 0;
    if (node->left) res += task03_rec(node->left, k);
    if (node->right) res += task03_rec(node->right, k);
    return res;
  }

  bool check_subtree_levels_less_k(rb_tree_node<T>* node, int k) {
    if (!node) return true;
    int level = 0;
    int node_count;
    while ((node_count = count_nodes_at_level(node, level)) > 0) {
      if (node_count > k) return false;
      level++;
    }
    return true;
  }

  int count_nodes_at_level(rb_tree_node<T>* node, int level) {
    if (!node) return 0;
    if (level == 0) return 1;
    return count_nodes_at_level(node->left, level - 1) + count_nodes_at_level(
               node->right, level - 1);
  }

  int count_total_in_subtree(rb_tree_node<T>* node) {
    if (!node) return 0;
    int total = node->get_length();
    if (node->left) total += count_total_in_subtree(node->left);
    if (node->right) total += count_total_in_subtree(node->right);
    return total;
  }

  int count_elements_at_level(rb_tree_node<T>* node, int level) {
    if (!node) return 0;
    if (level == 0) return node->get_length();
    return count_elements_at_level(node->left, level - 1) +
           count_elements_at_level(node->right, level - 1);
  }

  int leftmost_black_count(const rb_tree_node<T>* curr) const {
    int cnt = 0;
    while (curr) {
      if (curr->color == colors::black) cnt++;
      curr = curr->left;
    }
    return cnt;
  }

  int count_branches_with_depth(const rb_tree_node<T>* curr, const int k) const {
    if (!curr) return 0;
    if (k <= 0) {
      if (!curr->left && !curr->right) return 1;
      return 0;
    }
    int left = count_branches_with_depth(curr->left, k - 1);
    int right = count_branches_with_depth(curr->right, k - 1);
    int total = left + right;
    if (total != 0) total += 1;
    return total;
  }
};

template <class T>
io_status read_from_file(rb_tree<T>* tr, const char* name) {
  if (!tr) return io_status::format;
  FILE* f = fopen(name, "r");
  if (!f) return io_status::open;
  io_status res = tr->read(f);
  fclose(f);
  return res;
}

#endif