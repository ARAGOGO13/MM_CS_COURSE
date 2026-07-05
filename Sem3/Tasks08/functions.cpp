#include "tree.h"
#include <cstring>


void tree::swap_students(tree_node *a, tree_node *b) {
    if (a == nullptr || b == nullptr) return;
    student tmp = (student &&)(*a);
    (student &)(*a) = (student &&)(*b);
    (student &)(*b) = (student &&)tmp;
}

void tree::heapify_subtree(tree_node *node) {
    if (node == nullptr) return;
    if (node->left) heapify_subtree(node->left);
    if (node->right) heapify_subtree(node->right);
    tree_node *cur = node;
    while (true) {
        tree_node *largest = cur;
        if (cur->left && *(cur->left) > *largest) largest = cur->left;
        if (cur->right && *(cur->right) > *largest) largest = cur->right;
        if (largest == cur) break;
        swap_students(cur, largest);
        cur = largest;
    }
}

void tree::make_max() {
    if (root == nullptr) return;
    heapify_subtree(root);
}

tree_node *tree::node_at_level_index(tree_node *node, int level, int &index) {
    if (node == nullptr || level <= 0) return nullptr;
    if (level == 1) {
        if (index == 1) return node;
        --index;
        return nullptr;
    }
    tree_node *res = node_at_level_index(node->left, level - 1, index);
    if (res != nullptr) return res;
    return node_at_level_index(node->right, level - 1, index);
}

void tree::find_target_and_min_at_level(tree_node *node, int level, int target_pos,
                                        int &pos_counter,
                                        tree_node *&target_node,
                                        tree_node *&min_node) {
    if (node == nullptr) return;
    if (level == 1) {
        ++pos_counter;
        if (pos_counter == target_pos) target_node = node;
        if (pos_counter >= target_pos) {
            if (min_node == nullptr || node->get_value() < min_node->get_value())
                min_node = node;
        }
        return;
    }
    find_target_and_min_at_level(node->left, level - 1, target_pos, pos_counter, target_node, min_node);
    find_target_and_min_at_level(node->right, level - 1, target_pos, pos_counter, target_node, min_node);
}

void tree::sort_levels_by_value_min() {
    if (root == nullptr) return;
    int h = height_of_tree(root);
    for (int level = 1; level <= h; ++level) {
        int count = count_nodes_at_level(root, level);
        for (int pos = 1; pos <= count; ++pos) {
            int pos_counter = 0;
            tree_node *target = nullptr;
            tree_node *minNode = nullptr;
            find_target_and_min_at_level(root, level, pos, pos_counter, target, minNode);
            if (target && minNode && target != minNode) swap_students(target, minNode);
        }
    }
}

void tree::task3_transform() {
    if (root == nullptr) return;
    task3_transform_sub(root, nullptr);
}

void tree::task3_transform_sub(tree_node *node, tree_node *parent) {
    if (node == nullptr) return;
    tree_node *orig_left = node->left;
    tree_node *orig_right = node->right;
    if (node->left == nullptr && node->right != nullptr) {
        tree_node *A = node;
        tree_node *B = A->right;
        if (B->left != nullptr || B->right != nullptr) {
            tree_node *parentC = B;
            tree_node *C = B->left;
            while (C != nullptr && C->left != nullptr) {
                parentC = C;
                C = C->left;
            }
            if (C != nullptr && C->left == nullptr && C->right == nullptr) {
                if (parentC->left == C) parentC->left = nullptr;
                else if (parentC->right == C) parentC->right = nullptr;
                if (parent == nullptr) root = C;
                else {
                    if (parent->left == A) parent->left = C;
                    else if (parent->right == A) parent->right = C;
                }
                C->left = A;
                C->right = B;
                A->left = nullptr;
                A->right = nullptr;
                task3_transform_sub(C->left, C);
                task3_transform_sub(C->right, C);
                return;
            }
        }
    }
    task3_transform_sub(orig_left, node);
    task3_transform_sub(orig_right, node);
}

void tree::task4_transform() {
    if (root == nullptr) return;
    task4_transform_sub(root, nullptr);
}

void tree::task4_transform_sub(tree_node *node, tree_node *parent) {
    if (node == nullptr) return;
    tree_node *orig_left = node->left;
    tree_node *orig_right = node->right;
    if (node->right == nullptr && node->left != nullptr) {
        tree_node *A = node;
        tree_node *B = A->left;
        if (B->left != nullptr || B->right != nullptr) {
            tree_node *parentC = B;
            tree_node *C = B->right;
            while (C != nullptr && C->right != nullptr) {
                parentC = C;
                C = C->right;
            }
            if (C != nullptr && C->left == nullptr && C->right == nullptr) {
                if (parentC->left == C) parentC->left = nullptr;
                else if (parentC->right == C) parentC->right = nullptr;
                if (parent == nullptr) root = C;
                else {
                    if (parent->left == A) parent->left = C;
                    else if (parent->right == A) parent->right = C;
                }
                C->left = B;
                C->right = A;
                A->left = nullptr;
                A->right = nullptr;
                task4_transform_sub(C->left, C);
                task4_transform_sub(C->right, C);
                return;
            }
        }
    }
    task4_transform_sub(orig_left, node);
    task4_transform_sub(orig_right, node);
}

void tree::task5_transform(const student &x) {
    const char *s = x.get_name();
    int k = x.get_value();
    if (s == nullptr || s[0] == '\0' || k <= 0) return;
    if (root != nullptr) {
        int res = task5_rec(root, s, k);
        if (res >= k) {
            delete_subtree(root);
            root = nullptr;
        }
    }
}

int tree::task5_rec(tree_node *&root_node, const char *s, int k) {
    if (root_node == nullptr) return -1;
    int child_deep = -1;
    if (root_node->left != nullptr)
        child_deep = task5_rec(root_node->left, s, k);
    int deep = child_deep;
    if (root_node->right != nullptr)
        child_deep = task5_rec(root_node->right, s, k);
    const char *nm = root_node->get_name();
    int cmp = (nm != nullptr && s != nullptr && strstr(nm, s) != nullptr) ? 1 : 0;
    if ((deep < 0) && (child_deep < 0) && cmp) {
        deep = 1;
    } else if ((deep != 0) && (child_deep != 0) && cmp) {
        deep = ((child_deep < deep) ? deep : child_deep) + 1;
    } else {
        if (deep >= k) {
            delete_subtree(root_node->left);
            root_node->left = nullptr;
        }
        if (child_deep >= k) {
            delete_subtree(root_node->right);
            root_node->right = nullptr;
        }
        deep = 0;
    }
    return deep;
}

int tree::longest_match_depth_from_node(tree_node* curr_root, const char* pattern, int &flag) {
    if (curr_root == nullptr) { flag = 0; return -1; }
    int l1 = 0, l2 = 0;
    int flag1 = 1, flag2 = 1;
    const char* nm = curr_root->get_name();
    if (nm == nullptr || strstr(nm, pattern) == nullptr) {
        flag = 0;
        return -1;
    }
    if (curr_root->left) l1 = longest_match_depth_from_node(curr_root->left, pattern, flag1);
    if (curr_root->right) l2 = longest_match_depth_from_node(curr_root->right, pattern, flag2);
    if (flag1 && flag2) {
        return (l1 > l2 ? l1 : l2) + 1;
    } else if (flag1) {
        return l1 + 1;
    } else if (flag2) {
        return l2 + 1;
    } else {
        flag = 0;
        return -1;
    }
}

void tree::remove_subtree_with_deep_match(tree_node* parent, int is_lr, tree_node* curr_root, const char* pattern, int k) {
    if (curr_root == nullptr) return;
    int flag = 1;
    int kol = longest_match_depth_from_node(curr_root, pattern, flag);
    if (kol >= k && flag) {
        if (is_lr == 1) {
            parent->left = nullptr;
        } else if (is_lr == 2) {
            parent->right = nullptr;
        }
        delete_subtree(curr_root);
        if (is_lr == 0) {
            root = nullptr;
        }
        return;
    }
    if (curr_root->left) remove_subtree_with_deep_match(curr_root, 1, curr_root->left, pattern, k);
    if (curr_root->right) remove_subtree_with_deep_match(curr_root, 2, curr_root->right, pattern, k);
}

void tree::task6_transform(const student &x) {
    const char *s = x.get_name();
    int k = x.get_value();
    if (s == nullptr || s[0] == '\0' || k <= 0) return;
    if (root == nullptr) return;
    remove_subtree_with_deep_match(nullptr, 0, root, s, k);
}

int tree::count_nodes_and_longest_run_on_level(const char* pattern, int ur, int curr_ur, tree_node* curr_root, int &c, int &max_c) {
    if (curr_root == nullptr) return 0;
    int sum = 0;
    if (curr_ur == ur) {
        const char *nm = curr_root->get_name();
        if (nm == nullptr || strstr(nm, pattern) == nullptr) {
            if (c > max_c) max_c = c;
            c = 0;
        } else {
            c += 1;
        }
        return 1;
    }
    if (curr_root->left) sum += count_nodes_and_longest_run_on_level(pattern, ur, curr_ur + 1, curr_root->left, c, max_c);
    if (curr_root->right) sum += count_nodes_and_longest_run_on_level(pattern, ur, curr_ur + 1, curr_root->right, c, max_c);
    return sum;
}

void tree::remove_subtrees_starting_at_level(tree_node* curr_root, int ur, int curr_ur) {
    if (curr_root == nullptr) return;
    if (curr_ur + 1 == ur) {
        if (curr_root->left) {
            delete_subtree(curr_root->left);
            curr_root->left = nullptr;
        }
        if (curr_root->right) {
            delete_subtree(curr_root->right);
            curr_root->right = nullptr;
        }
    } else {
        if (curr_root->left) remove_subtrees_starting_at_level(curr_root->left, ur, curr_ur + 1);
        if (curr_root->right) remove_subtrees_starting_at_level(curr_root->right, ur, curr_ur + 1);
    }
}

void tree::task7_transform(const student &x) {
    const char *s = x.get_name();
    int k = x.get_value();
    if (s == nullptr || s[0] == '\0' || k <= 0) return;
    if (root == nullptr) return;
    int ur = 0;
    int c = 0;
    int max_c = 0;
    int is_pr = count_nodes_and_longest_run_on_level(s, ur, 0, root, c, max_c);
    if (c > max_c) max_c = c;
    while (is_pr != 0) {
        if (max_c >= k) {
            if (ur == 0) {
                delete_subtree(root);
                root = nullptr;
            } else {
                remove_subtrees_starting_at_level(root, ur, 0);
            }
            return;
        } else {
            ur += 1;
            c = 0;
            max_c = 0;
            is_pr = count_nodes_and_longest_run_on_level(s, ur, 0, root, c, max_c);
            if (c > max_c) max_c = c;
        }
    }
}
