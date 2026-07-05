#include "list.h"
#include <cstdio>
#include <new>

static bool less_by(const record &a, const record &b, const ordering ob[3]) {
    for (int i = 0; i < 3 && ob[i] != ordering::none; ++i) {
        if (a.is_less(b, ob[i])) return true;
        if (b.is_less(a, ob[i])) return false;
    }
    return false;
}

static void merge_sort(list_node **arr, list_node **temp, int n, const ordering ob[3]) {
    if (n <= 1) return;

    list_node **source = arr;
    list_node **dest = temp;

    for (int width = 1; width < n; width *= 2) {
        for (int i = 0; i < n; i += 2 * width) {
            int left = i;
            int mid = i + width;
            if (mid > n) mid = n;
            int right = i + 2 * width;
            if (right > n) right = n;

            int l = left;
            int r = mid;
            int k = left;

            while (l < mid && r < right) {
                if (less_by(*source[l], *source[r], ob)) {
                    dest[k++] = source[l++];
                } else {
                    dest[k++] = source[r++];
                }
            }
            while (l < mid) dest[k++] = source[l++];
            while (r < right) dest[k++] = source[r++];
        }
        list_node **swap = source;
        source = dest;
        dest = swap;
    }

    if (source != arr) {
        for (int i = 0; i < n; ++i) {
            arr[i] = source[i];
        }
    }
}

void list2::delete_list() {
    list_node *cur = head;
    while (cur) {
        list_node *next = cur->get_next();
        remove_node(cur);
        cur = next;
    }
}

void list2::remove_node(list_node *node) {
    index.remove(node);
    if (node->prev) node->prev->set_next(node->get_next());
    else head = node->get_next();
    if (node->next) node->next->set_prev(node->prev);
    else tail = node->prev;
    delete node;
}

io_status list2::read_from_file(const char *filename) {
    FILE *fp = fopen(filename, "r");
    if (!fp) return io_status::open;

    delete_list();

    while (true) {
        list_node *n = new(std::nothrow) list_node;
        if (!n) {
            delete_list();
            fclose(fp);
            return io_status::memory;
        }

        io_status st = n->read(fp);
        if (st == io_status::eof) {
            delete n;
            break;
        }
        if (st != io_status::success) {
            delete n;
            delete_list();
            fclose(fp);
            return st;
        }

        n->set_prev(tail);
        n->set_next(nullptr);
        if (tail) tail->set_next(n);
        tail = n;
        if (!head) head = n;
        index.insert(n);
    }

    fclose(fp);
    return io_status::success;
}

bool list2::insert(const record &r) {
    const char *nm = r.get_name();
    list_node *cur = index.get_bucket_head(nm);
    while (cur) {
        if (cur->equals_all(r)) {
            return false;
        }
        cur = cur->hash_next;
    }

    list_node *n = new(std::nothrow) list_node;
    if (!n) return false;

    if (n->init(r.get_name(), r.get_phone(), r.get_group()) != 0) {
        delete n;
        return false;
    }

    n->set_prev(tail);
    n->set_next(nullptr);
    if (tail) tail->set_next(n);
    tail = n;
    if (!head) head = n;

    index.insert(n);
    return true;
}

int list2::remove(const command &cmd) {
    int count = 0;

    bool use_index = false;
    const char *name_val = nullptr;
    condition name_cond = cmd.get_name_cond();
    operation op = cmd.get_op();
    if (cmd.get_type() == command_type::del &&
        name_cond == condition::eq &&
        (op == operation::none || op == operation::land)) {
        use_index = true;
        name_val = cmd.get_name();
    }

    if (!use_index) {
        list_node *cur = head;
        while (cur) {
            list_node *next = cur->get_next();
            if (cmd.apply(*cur)) {
                remove_node(cur);
                ++count;
            }
            cur = next;
        }
    } else {
        list_node *cur = index.get_bucket_head(name_val);
        while (cur) {
            list_node *next = cur->hash_next;
            if (cmd.apply(*cur)) {
                remove_node(cur);
                ++count;
            }
            cur = next;
        }
    }
    return count;
}

int list2::select_and_print(const command &cmd, FILE *fp) const {
    if (cmd.get_type() != command_type::select) return 0;

    const ordering *ob = cmd.get_order_by();
    bool need_sort = false;
    for (int i = 0; i < 3; ++i) {
        if (ob[i] != ordering::none) {
            need_sort = true;
            break;
        }
    }

    int found = 0;

    bool use_index = false;
    const char *name_val = nullptr;
    condition name_cond = cmd.get_name_cond();
    operation op = cmd.get_op();
    if (name_cond == condition::eq && (op == operation::none || op == operation::land)) {
        use_index = true;
        name_val = cmd.get_name();
    }

    if (!use_index) {
        if (!need_sort) {
            for (list_node *cur = head; cur; cur = cur->get_next()) {
                if (cmd.apply(*cur)) {
                    cur->print(cmd.get_order(), fp);
                    ++found;
                }
            }
        } else {
            int count = 0;
            for (list_node *cur = head; cur; cur = cur->get_next()) {
                if (cmd.apply(*cur)) ++count;
            }
            if (count == 0) {
                fprintf(fp, "\n");
                return 0;
            }

            list_node **arr = new(std::nothrow) list_node *[count];
            list_node **temp = new(std::nothrow) list_node *[count];

            if (!arr || !temp) {
                delete[] arr;
                delete[] temp;
                fprintf(fp, "\n");
                return 0;
            }

            int idx = 0;
            for (list_node *cur = head; cur; cur = cur->get_next()) {
                if (cmd.apply(*cur)) {
                    arr[idx++] = cur;
                }
            }

            merge_sort(arr, temp, count, ob);

            for (int i = 0; i < count; ++i) {
                arr[i]->print(cmd.get_order(), fp);
            }

            delete[] arr;
            delete[] temp;
            found = count;
        }
    } else {
        list_node *bucket = index.get_bucket_head(name_val);
        if (!need_sort) {
            for (list_node *cur = bucket; cur; cur = cur->hash_next) {
                if (cmd.apply(*cur)) {
                    cur->print(cmd.get_order(), fp);
                    ++found;
                }
            }
        } else {
            int count = 0;
            for (list_node *cur = bucket; cur; cur = cur->hash_next) {
                if (cmd.apply(*cur)) ++count;
            }
            if (count == 0) {
                fprintf(fp, "\n");
                return 0;
            }

            list_node **arr = new(std::nothrow) list_node *[count];
            list_node **temp = new(std::nothrow) list_node *[count];

            if (!arr || !temp) {
                delete[] arr;
                delete[] temp;
                fprintf(fp, "\n");
                return 0;
            }

            int idx = 0;
            for (list_node *cur = bucket; cur; cur = cur->hash_next) {
                if (cmd.apply(*cur)) {
                    arr[idx++] = cur;
                }
            }

            merge_sort(arr, temp, count, ob);

            for (int i = 0; i < count; ++i) {
                arr[i]->print(cmd.get_order(), fp);
            }

            delete[] arr;
            delete[] temp;
            found = count;
        }
    }

    fprintf(fp, "\n");
    return found;
}
