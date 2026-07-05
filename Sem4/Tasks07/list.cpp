#include "list.h"
#include <cstdio>
#include <new>
#include <cstring>

static long long stat_full_scan = 0;
static long long stat_name_index = 0;
static long long stat_phone_index = 0;
static long long stat_mixed = 0;

static bool less_by(const record &a, const record &b, const ordering ob[3]) {
    for (int i = 0; i < 3 && ob[i] != ordering::none; ++i) {
        switch (ob[i]) {
            case ordering::name: {
                const char *an = a.get_name() ? a.get_name() : "";
                const char *bn = b.get_name() ? b.get_name() : "";
                int c = std::strcmp(an, bn);
                if (c < 0) return true;
                if (c > 0) return false;
                break;
            }
            case ordering::phone:
                if (a.get_phone() < b.get_phone()) return true;
                if (a.get_phone() > b.get_phone()) return false;
                break;
            case ordering::group:
                if (a.get_group() < b.get_group()) return true;
                if (a.get_group() > b.get_group()) return false;
                break;
            case ordering::none:
                break;
        }
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
            int right = i + 2 * width;

            if (mid > n) mid = n;
            if (right > n) right = n;

            int l = left, r = mid, k = left;
            while (l < mid && r < right) {
                if (less_by(*source[l], *source[r], ob)) dest[k++] = source[l++];
                else dest[k++] = source[r++];
            }
            while (l < mid) dest[k++] = source[l++];
            while (r < right) dest[k++] = source[r++];
        }

        list_node **swap = source;
        source = dest;
        dest = swap;
    }

    if (source != arr) {
        for (int i = 0; i < n; ++i) arr[i] = source[i];
    }
}

static list_node *next_in_bucket(list_node *p, bool is_name_bucket) {
    return is_name_bucket ? p->name_hash_next : p->phone_hash_next;
}

static int bucket_len(list_node *p, bool is_name_bucket) {
    int n = 0;
    while (p) {
        ++n;
        p = next_in_bucket(p, is_name_bucket);
    }
    return n;
}

static bool contains_ptr_in_bucket(list_node *bucket, bool is_name_bucket, list_node *node) {
    while (bucket) {
        if (bucket == node) return true;
        bucket = next_in_bucket(bucket, is_name_bucket);
    }
    return false;
}

static int count_matches_in_bucket(list_node *bucket, bool is_name_bucket, const command &cmd) {
    int cnt = 0;
    for (list_node *cur = bucket; cur; cur = next_in_bucket(cur, is_name_bucket)) {
        if (cmd.apply(*cur)) ++cnt;
    }
    return cnt;
}

static int fill_matches_in_bucket(list_node *bucket, bool is_name_bucket, const command &cmd, list_node **out) {
    int idx = 0;
    for (list_node *cur = bucket; cur; cur = next_in_bucket(cur, is_name_bucket)) {
        if (cmd.apply(*cur)) out[idx++] = cur;
    }
    return idx;
}

static int count_union_matches(list_node *a, bool a_is_name, list_node *b, bool b_is_name, const command &cmd) {
    int cnt = 0;
    for (list_node *cur = a; cur; cur = next_in_bucket(cur, a_is_name)) {
        if (cmd.apply(*cur)) ++cnt;
    }
    for (list_node *cur = b; cur; cur = next_in_bucket(cur, b_is_name)) {
        if (cmd.apply(*cur) && !contains_ptr_in_bucket(a, a_is_name, cur)) ++cnt;
    }
    return cnt;
}

static int fill_union_matches(list_node *a, bool a_is_name, list_node *b, bool b_is_name, const command &cmd,
                              list_node **out) {
    int idx = 0;
    for (list_node *cur = a; cur; cur = next_in_bucket(cur, a_is_name)) {
        if (cmd.apply(*cur)) out[idx++] = cur;
    }
    for (list_node *cur = b; cur; cur = next_in_bucket(cur, b_is_name)) {
        if (cmd.apply(*cur) && !contains_ptr_in_bucket(a, a_is_name, cur)) {
            out[idx++] = cur;
        }
    }
    return idx;
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
    name_index.remove(node);
    phone_index.remove(node);

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

        name_index.insert(n);
        phone_index.insert(n);
    }

    fclose(fp);
    return io_status::success;
}

bool list2::insert(const record &r) {
    const char *nm = r.get_name();
    list_node *cur = name_index.get_bucket_head(nm);

    while (cur) {
        if (cur->compare_name(condition::eq, r) &&
            cur->compare_phone(condition::eq, r) &&
            cur->compare_group(condition::eq, r)) {
            return false;
        }
        cur = cur->name_hash_next;
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

    name_index.insert(n);
    phone_index.insert(n);
    return true;
}

int list2::remove(const command &cmd) {
    int count = 0;
    const bool has_name_eq = (cmd.get_name_cond() == condition::eq);
    const bool has_phone_eq = (cmd.get_phone_cond() == condition::eq);

    if (cmd.get_op() == operation::land && has_name_eq && has_phone_eq) {
        stat_mixed++;

        list_node *name_bucket = name_index.get_bucket_head(cmd.get_name());
        list_node *phone_bucket = phone_index.get_bucket_head(cmd.get_phone());

        bool name_is_smaller = bucket_len(name_bucket, true) <= bucket_len(phone_bucket, false);
        list_node *bucket = name_is_smaller ? name_bucket : phone_bucket;
        bool bucket_is_name = name_is_smaller;

        if (bucket_is_name) stat_name_index++;
        else stat_phone_index++;

        while (bucket) {
            list_node *next = next_in_bucket(bucket, bucket_is_name);
            if (cmd.apply(*bucket)) {
                remove_node(bucket);
                ++count;
            }
            bucket = next;
        }
        return count;
    }

    if (cmd.get_op() == operation::lor && has_name_eq && has_phone_eq) {
        stat_mixed++;
        stat_name_index++;
        stat_phone_index++;

        list_node *name_bucket = name_index.get_bucket_head(cmd.get_name());
        list_node *phone_bucket = phone_index.get_bucket_head(cmd.get_phone());

        bool name_is_smaller = bucket_len(name_bucket, true) <= bucket_len(phone_bucket, false);

        list_node *a = name_is_smaller ? name_bucket : phone_bucket;
        bool a_is_name = name_is_smaller;
        list_node *b = name_is_smaller ? phone_bucket : name_bucket;
        bool b_is_name = !name_is_smaller;

        int total = count_union_matches(a, a_is_name, b, b_is_name, cmd);
        if (total == 0) return 0;

        list_node **arr = new(std::nothrow) list_node *[total];
        if (!arr) return 0;

        fill_union_matches(a, a_is_name, b, b_is_name, cmd, arr);

        for (int i = 0; i < total; ++i) {
            remove_node(arr[i]);
            ++count;
        }

        delete[] arr;
        return count;
    }

    if (has_name_eq) {
        stat_name_index++;
        list_node *cur = name_index.get_bucket_head(cmd.get_name());
        while (cur) {
            list_node *next = cur->name_hash_next;
            if (cmd.apply(*cur)) {
                remove_node(cur);
                ++count;
            }
            cur = next;
        }
        return count;
    }

    if (has_phone_eq) {
        stat_phone_index++;
        list_node *cur = phone_index.get_bucket_head(cmd.get_phone());
        while (cur) {
            list_node *next = cur->phone_hash_next;
            if (cmd.apply(*cur)) {
                remove_node(cur);
                ++count;
            }
            cur = next;
        }
        return count;
    }

    stat_full_scan++;
    list_node *cur = head;
    while (cur) {
        list_node *next = cur->get_next();
        if (cmd.apply(*cur)) {
            remove_node(cur);
            ++count;
        }
        cur = next;
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
    const bool has_name_eq = (cmd.get_name_cond() == condition::eq);
    const bool has_phone_eq = (cmd.get_phone_cond() == condition::eq);

    if (cmd.get_op() == operation::lor && has_name_eq && has_phone_eq) {
        stat_mixed++;
        stat_name_index++;
        stat_phone_index++;

        list_node *name_bucket = name_index.get_bucket_head(cmd.get_name());
        list_node *phone_bucket = phone_index.get_bucket_head(cmd.get_phone());

        bool name_is_smaller = bucket_len(name_bucket, true) <= bucket_len(phone_bucket, false);
        list_node *a = name_is_smaller ? name_bucket : phone_bucket;
        bool a_is_name = name_is_smaller;
        list_node *b = name_is_smaller ? phone_bucket : name_bucket;
        bool b_is_name = !name_is_smaller;

        if (!need_sort) {
            for (list_node *p = a; p; p = next_in_bucket(p, a_is_name)) {
                if (cmd.apply(*p)) {
                    p->print(cmd.get_order(), fp);
                    ++found;
                }
            }
            for (list_node *p = b; p; p = next_in_bucket(p, b_is_name)) {
                if (!contains_ptr_in_bucket(a, a_is_name, p) && cmd.apply(*p)) {
                    p->print(cmd.get_order(), fp);
                    ++found;
                }
            }
            fprintf(fp, "\n");
            return found;
        }

        int total = count_union_matches(a, a_is_name, b, b_is_name, cmd);
        if (total == 0) {
            fprintf(fp, "\n");
            return 0;
        }

        list_node **arr = new(std::nothrow) list_node *[total];
        list_node **temp = new(std::nothrow) list_node *[total];
        if (!arr || !temp) {
            delete[] arr;
            delete[] temp;
            fprintf(fp, "\n");
            return 0;
        }

        fill_union_matches(a, a_is_name, b, b_is_name, cmd, arr);
        merge_sort(arr, temp, total, ob);

        for (int i = 0; i < total; ++i) {
            arr[i]->print(cmd.get_order(), fp);
        }

        delete[] arr;
        delete[] temp;
        fprintf(fp, "\n");
        return total;
    }

    if (cmd.get_op() == operation::land && has_name_eq && has_phone_eq) {
        stat_mixed++;

        list_node *name_bucket = name_index.get_bucket_head(cmd.get_name());
        list_node *phone_bucket = phone_index.get_bucket_head(cmd.get_phone());

        bool name_is_smaller = bucket_len(name_bucket, true) <= bucket_len(phone_bucket, false);
        list_node *bucket = name_is_smaller ? name_bucket : phone_bucket;
        bool bucket_is_name = name_is_smaller;

        if (bucket_is_name) stat_name_index++;
        else stat_phone_index++;

        if (!need_sort) {
            for (list_node *p = bucket; p; p = next_in_bucket(p, bucket_is_name)) {
                if (cmd.apply(*p)) {
                    p->print(cmd.get_order(), fp);
                    ++found;
                }
            }
            fprintf(fp, "\n");
            return found;
        }

        int cnt = count_matches_in_bucket(bucket, bucket_is_name, cmd);
        if (cnt == 0) {
            fprintf(fp, "\n");
            return 0;
        }

        list_node **arr = new(std::nothrow) list_node *[cnt];
        list_node **temp = new(std::nothrow) list_node *[cnt];
        if (!arr || !temp) {
            delete[] arr;
            delete[] temp;
            fprintf(fp, "\n");
            return 0;
        }

        fill_matches_in_bucket(bucket, bucket_is_name, cmd, arr);
        merge_sort(arr, temp, cnt, ob);

        for (int i = 0; i < cnt; ++i) {
            arr[i]->print(cmd.get_order(), fp);
        }

        delete[] arr;
        delete[] temp;
        fprintf(fp, "\n");
        return cnt;
    }

    if (has_name_eq) {
        stat_name_index++;
        list_node *bucket = name_index.get_bucket_head(cmd.get_name());

        if (!need_sort) {
            for (list_node *p = bucket; p; p = p->name_hash_next) {
                if (cmd.apply(*p)) {
                    p->print(cmd.get_order(), fp);
                    ++found;
                }
            }
            fprintf(fp, "\n");
            return found;
        }

        int cnt = count_matches_in_bucket(bucket, true, cmd);
        if (cnt == 0) {
            fprintf(fp, "\n");
            return 0;
        }

        list_node **arr = new(std::nothrow) list_node *[cnt];
        list_node **temp = new(std::nothrow) list_node *[cnt];
        if (!arr || !temp) {
            delete[] arr;
            delete[] temp;
            fprintf(fp, "\n");
            return 0;
        }

        fill_matches_in_bucket(bucket, true, cmd, arr);
        merge_sort(arr, temp, cnt, ob);

        for (int i = 0; i < cnt; ++i) {
            arr[i]->print(cmd.get_order(), fp);
        }

        delete[] arr;
        delete[] temp;
        fprintf(fp, "\n");
        return cnt;
    }

    if (has_phone_eq) {
        stat_phone_index++;
        list_node *bucket = phone_index.get_bucket_head(cmd.get_phone());

        if (!need_sort) {
            for (list_node *p = bucket; p; p = p->phone_hash_next) {
                if (cmd.apply(*p)) {
                    p->print(cmd.get_order(), fp);
                    ++found;
                }
            }
            fprintf(fp, "\n");
            return found;
        }

        int cnt = count_matches_in_bucket(bucket, false, cmd);
        if (cnt == 0) {
            fprintf(fp, "\n");
            return 0;
        }

        list_node **arr = new(std::nothrow) list_node *[cnt];
        list_node **temp = new(std::nothrow) list_node *[cnt];
        if (!arr || !temp) {
            delete[] arr;
            delete[] temp;
            fprintf(fp, "\n");
            return 0;
        }

        fill_matches_in_bucket(bucket, false, cmd, arr);
        merge_sort(arr, temp, cnt, ob);

        for (int i = 0; i < cnt; ++i) {
            arr[i]->print(cmd.get_order(), fp);
        }

        delete[] arr;
        delete[] temp;
        fprintf(fp, "\n");
        return cnt;
    }

    stat_full_scan++;
    int cnt = 0;
    for (list_node *p = head; p; p = p->get_next()) {
        if (cmd.apply(*p)) ++cnt;
    }

    if (cnt == 0) {
        fprintf(fp, "\n");
        return 0;
    }

    list_node **arr = new(std::nothrow) list_node *[cnt];
    list_node **temp = new(std::nothrow) list_node *[cnt];
    if (!arr || !temp) {
        delete[] arr;
        delete[] temp;
        fprintf(fp, "\n");
        return 0;
    }

    int idx = 0;
    for (list_node *p = head; p; p = p->get_next()) {
        if (cmd.apply(*p)) arr[idx++] = p;
    }

    if (need_sort) {
        merge_sort(arr, temp, cnt, ob);
    }

    for (int i = 0; i < cnt; ++i) {
        arr[i]->print(cmd.get_order(), fp);
    }

    delete[] arr;
    delete[] temp;
    fprintf(fp, "\n");
    return cnt;
}

void list2::print_stats() const {
    printf("STAT: full_scan=%lld | name_index=%lld | phone_index=%lld | mixed=%lld\n",
           stat_full_scan, stat_name_index, stat_phone_index, stat_mixed);
}
