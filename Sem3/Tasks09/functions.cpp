#include "tree.h"

int tree::task01(int k) {
    return t1(root, k);
}

int tree::t1(tree_node* n, int k) {
    if (!n) return 0;
    int cnt = 0;
    int ch_cnt = 0;
    tree_node* ch = n->down;
    while (ch) {
        ch_cnt++;
        cnt += t1(ch, k);
        ch = ch->level;
    }
    if (ch_cnt == k) cnt++;
    return cnt;
}

int tree::task02(int k) {
    int sz;
    return t2(root, k, &sz);
}

int tree::t2(tree_node* n, int k, int* sz_out) {
    if (!n) {
        *sz_out = 0;
        return 0;
    }
    int res = 0;
    int total = 1;
    tree_node* ch = n->down;
    while (ch) {
        int ch_sz;
        res += t2(ch, k, &ch_sz);
        total += ch_sz;
        ch = ch->level;
    }
    if (total <= k) res = total;
    *sz_out = total;
    return res;
}

int tree::task03(int k) {
    int h, sz;
    return t3(root, k, &h, &sz);
}

int tree::t3(tree_node* n, int k, int* h_out, int* sz_out) {
    if (!n) {
        *h_out = 0;
        *sz_out = 0;
        return 0;
    }
    int res = 0;
    int total = 1;
    int max_h = 0;
    tree_node* ch = n->down;
    while (ch) {
        int ch_h, ch_sz;
        res += t3(ch, k, &ch_h, &ch_sz);
        if (ch_h > max_h) max_h = ch_h;
        total += ch_sz;
        ch = ch->level;
    }
    if (max_h < k) res = total;
    *h_out = max_h + 1;
    *sz_out = total;
    return res;
}

int tree::task04(int k) {
    return t4(root, k);
}

int tree::t4(tree_node* n, int k) {
    if (!n) return 0;
    if (check_lvls(n, k)) {
        return cnt_all(n);
    }
    int res = 0;
    tree_node* ch = n->down;
    while (ch) {
        res += t4(ch, k);
        ch = ch->level;
    }
    return res;
}

bool tree::check_lvls(tree_node* n, int k) {
    if (!n) return 1;
    int lvl = 0;
    int cnt;
    while ((cnt = cnt_lvl(n, lvl)) > 0) {
        if (cnt > k) return 0;
        lvl++;
    }
    return 1;
}

int tree::cnt_lvl(tree_node* n, int lvl) {
    if (!n) return 0;
    if (lvl == 0) return 1;
    int c = 0;
    tree_node* ch = n->down;
    while (ch) {
        c += cnt_lvl(ch, lvl - 1);
        ch = ch->level;
    }
    return c;
}

int tree::cnt_all(tree_node* n) {
    if (!n) return 0;
    int c = 1;
    tree_node* ch = n->down;
    while (ch) {
        c += cnt_all(ch);
        ch = ch->level;
    }
    return c;
}

int tree::task05(int k) {
    return cnt_lvl(root, k);
}

int tree::task06(int k) {
    int h;
    return t6(root, k, &h);
}

int tree::t6(tree_node* n, int k, int* h_out) {
    if (!n) {
        *h_out = 0;
        return 0;
    }
    int res = 0;
    int max_ch_h = 0;
    tree_node* ch = n->down;
    while (ch) {
        int ch_h;
        res += t6(ch, k - 1, &ch_h);
        if (ch_h > max_ch_h) max_ch_h = ch_h;
        ch = ch->level;
    }
    if (max_ch_h >= k - 1) res++;
    *h_out = max_ch_h + 1;
    return res;
}

int tree::task07(int k) {
    return t7(&root, k);
}

int tree::t7(tree_node** np, int k) {
    if (!*np) return 0;
    tree_node* n = *np;

    if (n->get_value() <= k) {
        int del_cnt = cnt_leq(n, k);
        delete_subtree(n);
        *np = 0;
        return del_cnt;
    }

    int total_del = 0;
    tree_node* prev = 0;
    tree_node* ch = n->down;
    while (ch) {
        tree_node* nxt = ch->level;
        total_del += t7(&ch, k);
        if (!ch) {
            if (prev) prev->level = nxt;
            else n->down = nxt;
        } else {
            prev = ch;
        }
        ch = nxt;
    }
    return total_del;
}

int tree::cnt_leq(tree_node* n, int k) {
    if (!n) return 0;
    int c = 0;
    tree_node* ch = n->down;
    while (ch) {
        c += cnt_leq(ch, k);
        ch = ch->level;
    }
    if (n->get_value() <= k) c++;
    return c;
}