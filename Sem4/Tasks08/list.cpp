#include "list.h"

#include <cstdio>
#include <cstring>
#include <new>

#include "command.h"

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

static void merge_sort(list_node **arr, list_node **temp, int n,
                       const ordering ob[3]) {
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
        if (less_by(*source[l], *source[r], ob))
          dest[k++] = source[l++];
        else
          dest[k++] = source[r++];
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

static list_node *next_in_bucket(list_node *p, bool is_name_bucket,
                                 bool is_group_hash) {
  if (is_group_hash) {
    return is_name_bucket ? p->g_name_hash_next : p->g_phone_hash_next;
  }
  return is_name_bucket ? p->name_hash_next : p->phone_hash_next;
}

static int bucket_len(list_node *p, bool is_name_bucket, bool is_group_hash) {
  int n = 0;
  while (p) {
    ++n;
    p = next_in_bucket(p, is_name_bucket, is_group_hash);
  }
  return n;
}

static bool contains_ptr_in_bucket(list_node *bucket, bool is_name_bucket,
                                   bool is_group_hash, list_node *node) {
  while (bucket) {
    if (bucket == node) return true;
    bucket = next_in_bucket(bucket, is_name_bucket, is_group_hash);
  }
  return false;
}

static int count_matches_in_bucket(list_node *bucket, bool is_name_bucket,
                                   bool is_group_hash, const command &cmd) {
  int cnt = 0;
  for (list_node *cur = bucket; cur;
       cur = next_in_bucket(cur, is_name_bucket, is_group_hash)) {
    if (cmd.apply(*cur)) ++cnt;
  }
  return cnt;
}

static int fill_matches_in_bucket(list_node *bucket, bool is_name_bucket,
                                  bool is_group_hash, const command &cmd,
                                  list_node **out) {
  int idx = 0;
  for (list_node *cur = bucket; cur;
       cur = next_in_bucket(cur, is_name_bucket, is_group_hash)) {
    if (cmd.apply(*cur)) out[idx++] = cur;
  }
  return idx;
}

static int count_union_matches(list_node *a, bool a_is_name, bool a_is_group,
                               list_node *b, bool b_is_name, bool b_is_group,
                               const command &cmd) {
  int cnt = 0;
  for (list_node *cur = a; cur;
       cur = next_in_bucket(cur, a_is_name, a_is_group)) {
    if (cmd.apply(*cur)) ++cnt;
  }
  for (list_node *cur = b; cur;
       cur = next_in_bucket(cur, b_is_name, b_is_group)) {
    if (cmd.apply(*cur) &&
        !contains_ptr_in_bucket(a, a_is_name, a_is_group, cur))
      ++cnt;
  }
  return cnt;
}

static int fill_union_matches(list_node *a, bool a_is_name, bool a_is_group,
                              list_node *b, bool b_is_name, bool b_is_group,
                              const command &cmd, list_node **out) {
  int idx = 0;
  for (list_node *cur = a; cur;
       cur = next_in_bucket(cur, a_is_name, a_is_group)) {
    if (cmd.apply(*cur)) out[idx++] = cur;
  }
  for (list_node *cur = b; cur;
       cur = next_in_bucket(cur, b_is_name, b_is_group)) {
    if (cmd.apply(*cur) &&
        !contains_ptr_in_bucket(a, a_is_name, a_is_group, cur)) {
      out[idx++] = cur;
    }
  }
  return idx;
}

list2::list2(int k_name, int k_phone)
    : name_index(k_name, false), phone_index(k_phone, false) {
  for (int i = 0; i < 1000; ++i) {
    group_name_indexes[i] = new hash_index<const char *>(k_name, true);
    group_phone_indexes[i] = new hash_index<int>(k_phone, true);
    group_heads[i] = nullptr;
    group_tails[i] = nullptr;
  }
}

list2::~list2() {
  delete_list();
  for (int i = 0; i < 1000; ++i) {
    delete group_name_indexes[i];
    delete group_phone_indexes[i];
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
  name_index.remove(node);
  phone_index.remove(node);
  int g = node->get_group();
  if (g >= 0 && g < 1000) {
    group_name_indexes[g]->remove(node);
    group_phone_indexes[g]->remove(node);
    if (node->group_prev)
      node->group_prev->group_next = node->group_next;
    else
      group_heads[g] = node->group_next;
    if (node->group_next)
      node->group_next->group_prev = node->group_prev;
    else
      group_tails[g] = node->group_prev;
  }
  if (node->prev)
    node->prev->set_next(node->get_next());
  else
    head = node->get_next();
  if (node->next)
    node->next->set_prev(node->get_prev());
  else
    tail = node->get_prev();
  delete node;
}

io_status list2::read_from_file(const char *filename) {
  FILE *fp = fopen(filename, "r");
  if (!fp) return io_status::open;
  delete_list();
  while (true) {
    list_node *n = new (std::nothrow) list_node;
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
    int g = n->get_group();
    if (g >= 0 && g < 1000) {
      group_name_indexes[g]->insert(n);
      group_phone_indexes[g]->insert(n);
      n->group_prev = group_tails[g];
      n->group_next = nullptr;
      if (group_tails[g]) group_tails[g]->group_next = n;
      group_tails[g] = n;
      if (!group_heads[g]) group_heads[g] = n;
    }
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
  list_node *n = new (std::nothrow) list_node;
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
  int g = n->get_group();
  if (g >= 0 && g < 1000) {
    group_name_indexes[g]->insert(n);
    group_phone_indexes[g]->insert(n);
    n->group_prev = group_tails[g];
    n->group_next = nullptr;
    if (group_tails[g]) group_tails[g]->group_next = n;
    group_tails[g] = n;
    if (!group_heads[g]) group_heads[g] = n;
  }
  return true;
}

int list2::remove(const command &cmd) {
  int count = 0;
  const bool has_name_eq = (cmd.get_name_cond() == condition::eq);
  const bool has_phone_eq = (cmd.get_phone_cond() == condition::eq);
  const bool has_group_eq = (cmd.get_group_cond() == condition::eq);
  int g = has_group_eq ? cmd.get_group() : -1;
  bool use_group = has_group_eq && g >= 0 && g < 1000;
  if (use_group) {
    if (cmd.get_op() == operation::lor) {
      list_node *cur = group_heads[g];
      while (cur) {
        list_node *next = cur->group_next;
        remove_node(cur);
        ++count;
        cur = next;
      }
      return count;
    }
    if (cmd.get_op() == operation::land && has_name_eq && has_phone_eq) {
      stat_mixed++;
      list_node *name_bucket =
          group_name_indexes[g]->get_bucket_head(cmd.get_name());
      list_node *phone_bucket =
          group_phone_indexes[g]->get_bucket_head(cmd.get_phone());
      bool name_is_smaller = bucket_len(name_bucket, true, true) <=
                             bucket_len(phone_bucket, false, true);
      list_node *bucket = name_is_smaller ? name_bucket : phone_bucket;
      bool bucket_is_name = name_is_smaller;
      if (bucket_is_name)
        stat_name_index++;
      else
        stat_phone_index++;
      while (bucket) {
        list_node *next = next_in_bucket(bucket, bucket_is_name, true);
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
      list_node *name_bucket =
          group_name_indexes[g]->get_bucket_head(cmd.get_name());
      list_node *phone_bucket =
          group_phone_indexes[g]->get_bucket_head(cmd.get_phone());
      bool name_is_smaller = bucket_len(name_bucket, true, true) <=
                             bucket_len(phone_bucket, false, true);
      list_node *a = name_is_smaller ? name_bucket : phone_bucket;
      bool a_is_name = name_is_smaller;
      list_node *b = name_is_smaller ? phone_bucket : name_bucket;
      bool b_is_name = !name_is_smaller;
      int total =
          count_union_matches(a, a_is_name, true, b, b_is_name, true, cmd);
      if (total == 0) return 0;
      list_node **arr = new (std::nothrow) list_node *[total];
      if (!arr) return 0;
      fill_union_matches(a, a_is_name, true, b, b_is_name, true, cmd, arr);
      for (int i = 0; i < total; ++i) {
        remove_node(arr[i]);
        ++count;
      }
      delete[] arr;
      return count;
    }
    if (has_name_eq) {
      stat_name_index++;
      list_node *cur = group_name_indexes[g]->get_bucket_head(cmd.get_name());
      while (cur) {
        list_node *next = cur->g_name_hash_next;
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
      list_node *cur = group_phone_indexes[g]->get_bucket_head(cmd.get_phone());
      while (cur) {
        list_node *next = cur->g_phone_hash_next;
        if (cmd.apply(*cur)) {
          remove_node(cur);
          ++count;
        }
        cur = next;
      }
      return count;
    }
    list_node *cur = group_heads[g];
    while (cur) {
      list_node *next = cur->group_next;
      if (cmd.apply(*cur)) {
        remove_node(cur);
        ++count;
      }
      cur = next;
    }
    return count;
  }
  if (cmd.get_op() == operation::land && has_name_eq && has_phone_eq) {
    stat_mixed++;
    list_node *name_bucket = name_index.get_bucket_head(cmd.get_name());
    list_node *phone_bucket = phone_index.get_bucket_head(cmd.get_phone());
    bool name_is_smaller = bucket_len(name_bucket, true, false) <=
                           bucket_len(phone_bucket, false, false);
    list_node *bucket = name_is_smaller ? name_bucket : phone_bucket;
    bool bucket_is_name = name_is_smaller;
    if (bucket_is_name)
      stat_name_index++;
    else
      stat_phone_index++;
    while (bucket) {
      list_node *next = next_in_bucket(bucket, bucket_is_name, false);
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
    bool name_is_smaller = bucket_len(name_bucket, true, false) <=
                           bucket_len(phone_bucket, false, false);
    list_node *a = name_is_smaller ? name_bucket : phone_bucket;
    bool a_is_name = name_is_smaller;
    list_node *b = name_is_smaller ? phone_bucket : name_bucket;
    bool b_is_name = !name_is_smaller;
    int total =
        count_union_matches(a, a_is_name, false, b, b_is_name, false, cmd);
    if (total == 0) return 0;
    list_node **arr = new (std::nothrow) list_node *[total];
    if (!arr) return 0;
    fill_union_matches(a, a_is_name, false, b, b_is_name, false, cmd, arr);
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
  const bool has_group_eq = (cmd.get_group_cond() == condition::eq);
  int g = has_group_eq ? cmd.get_group() : -1;
  bool use_group = has_group_eq && g >= 0 && g < 1000;
  if (use_group) {
    if (cmd.get_op() == operation::lor) {
      if (!need_sort) {
        for (list_node *p = group_heads[g]; p; p = p->group_next) {
          p->print(cmd.get_order(), fp);
          ++found;
        }
        fprintf(fp, "\n");
        return found;
      }
      int cnt = 0;
      for (list_node *p = group_heads[g]; p; p = p->group_next) ++cnt;
      if (cnt == 0) {
        fprintf(fp, "\n");
        return 0;
      }
      list_node **arr = new (std::nothrow) list_node *[cnt];
      list_node **temp = new (std::nothrow) list_node *[cnt];
      if (!arr || !temp) {
        delete[] arr;
        delete[] temp;
        fprintf(fp, "\n");
        return 0;
      }
      int idx = 0;
      for (list_node *p = group_heads[g]; p; p = p->group_next) arr[idx++] = p;
      merge_sort(arr, temp, cnt, ob);
      for (int i = 0; i < cnt; ++i) arr[i]->print(cmd.get_order(), fp);
      delete[] arr;
      delete[] temp;
      fprintf(fp, "\n");
      return cnt;
    }
    if (cmd.get_op() == operation::land && has_name_eq && has_phone_eq) {
      stat_mixed++;
      list_node *name_bucket =
          group_name_indexes[g]->get_bucket_head(cmd.get_name());
      list_node *phone_bucket =
          group_phone_indexes[g]->get_bucket_head(cmd.get_phone());
      bool name_is_smaller = bucket_len(name_bucket, true, true) <=
                             bucket_len(phone_bucket, false, true);
      list_node *bucket = name_is_smaller ? name_bucket : phone_bucket;
      bool bucket_is_name = name_is_smaller;
      if (bucket_is_name)
        stat_name_index++;
      else
        stat_phone_index++;
      if (!need_sort) {
        for (list_node *p = bucket; p;
             p = next_in_bucket(p, bucket_is_name, true)) {
          if (cmd.apply(*p)) {
            p->print(cmd.get_order(), fp);
            ++found;
          }
        }
        fprintf(fp, "\n");
        return found;
      }
      int cnt = count_matches_in_bucket(bucket, bucket_is_name, true, cmd);
      if (cnt == 0) {
        fprintf(fp, "\n");
        return 0;
      }
      list_node **arr = new (std::nothrow) list_node *[cnt];
      list_node **temp = new (std::nothrow) list_node *[cnt];
      if (!arr || !temp) {
        delete[] arr;
        delete[] temp;
        fprintf(fp, "\n");
        return 0;
      }
      fill_matches_in_bucket(bucket, bucket_is_name, true, cmd, arr);
      merge_sort(arr, temp, cnt, ob);
      for (int i = 0; i < cnt; ++i) arr[i]->print(cmd.get_order(), fp);
      delete[] arr;
      delete[] temp;
      fprintf(fp, "\n");
      return cnt;
    }
    if (cmd.get_op() == operation::lor && has_name_eq && has_phone_eq) {
      stat_mixed++;
      stat_name_index++;
      stat_phone_index++;
      list_node *name_bucket =
          group_name_indexes[g]->get_bucket_head(cmd.get_name());
      list_node *phone_bucket =
          group_phone_indexes[g]->get_bucket_head(cmd.get_phone());
      bool name_is_smaller = bucket_len(name_bucket, true, true) <=
                             bucket_len(phone_bucket, false, true);
      list_node *a = name_is_smaller ? name_bucket : phone_bucket;
      bool a_is_name = name_is_smaller;
      list_node *b = name_is_smaller ? phone_bucket : name_bucket;
      bool b_is_name = !name_is_smaller;
      if (!need_sort) {
        for (list_node *p = a; p; p = next_in_bucket(p, a_is_name, true)) {
          if (cmd.apply(*p)) {
            p->print(cmd.get_order(), fp);
            ++found;
          }
        }
        for (list_node *p = b; p; p = next_in_bucket(p, b_is_name, true)) {
          if (!contains_ptr_in_bucket(a, a_is_name, true, p) && cmd.apply(*p)) {
            p->print(cmd.get_order(), fp);
            ++found;
          }
        }
        fprintf(fp, "\n");
        return found;
      }
      int total =
          count_union_matches(a, a_is_name, true, b, b_is_name, true, cmd);
      if (total == 0) {
        fprintf(fp, "\n");
        return 0;
      }
      list_node **arr = new (std::nothrow) list_node *[total];
      list_node **temp = new (std::nothrow) list_node *[total];
      if (!arr || !temp) {
        delete[] arr;
        delete[] temp;
        fprintf(fp, "\n");
        return 0;
      }
      fill_union_matches(a, a_is_name, true, b, b_is_name, true, cmd, arr);
      merge_sort(arr, temp, total, ob);
      for (int i = 0; i < total; ++i) arr[i]->print(cmd.get_order(), fp);
      delete[] arr;
      delete[] temp;
      fprintf(fp, "\n");
      return total;
    }
    if (has_name_eq) {
      stat_name_index++;
      list_node *bucket =
          group_name_indexes[g]->get_bucket_head(cmd.get_name());
      if (!need_sort) {
        for (list_node *p = bucket; p; p = p->g_name_hash_next) {
          if (cmd.apply(*p)) {
            p->print(cmd.get_order(), fp);
            ++found;
          }
        }
        fprintf(fp, "\n");
        return found;
      }
      int cnt = count_matches_in_bucket(bucket, true, true, cmd);
      if (cnt == 0) {
        fprintf(fp, "\n");
        return 0;
      }
      list_node **arr = new (std::nothrow) list_node *[cnt];
      list_node **temp = new (std::nothrow) list_node *[cnt];
      if (!arr || !temp) {
        delete[] arr;
        delete[] temp;
        fprintf(fp, "\n");
        return 0;
      }
      fill_matches_in_bucket(bucket, true, true, cmd, arr);
      merge_sort(arr, temp, cnt, ob);
      for (int i = 0; i < cnt; ++i) arr[i]->print(cmd.get_order(), fp);
      delete[] arr;
      delete[] temp;
      fprintf(fp, "\n");
      return cnt;
    }
    if (has_phone_eq) {
      stat_phone_index++;
      list_node *bucket =
          group_phone_indexes[g]->get_bucket_head(cmd.get_phone());
      if (!need_sort) {
        for (list_node *p = bucket; p; p = p->g_phone_hash_next) {
          if (cmd.apply(*p)) {
            p->print(cmd.get_order(), fp);
            ++found;
          }
        }
        fprintf(fp, "\n");
        return found;
      }
      int cnt = count_matches_in_bucket(bucket, false, true, cmd);
      if (cnt == 0) {
        fprintf(fp, "\n");
        return 0;
      }
      list_node **arr = new (std::nothrow) list_node *[cnt];
      list_node **temp = new (std::nothrow) list_node *[cnt];
      if (!arr || !temp) {
        delete[] arr;
        delete[] temp;
        fprintf(fp, "\n");
        return 0;
      }
      fill_matches_in_bucket(bucket, false, true, cmd, arr);
      merge_sort(arr, temp, cnt, ob);
      for (int i = 0; i < cnt; ++i) arr[i]->print(cmd.get_order(), fp);
      delete[] arr;
      delete[] temp;
      fprintf(fp, "\n");
      return cnt;
    }
    if (!need_sort) {
      for (list_node *p = group_heads[g]; p; p = p->group_next) {
        if (cmd.apply(*p)) {
          p->print(cmd.get_order(), fp);
          ++found;
        }
      }
      fprintf(fp, "\n");
      return found;
    }
    int cnt = 0;
    for (list_node *p = group_heads[g]; p; p = p->group_next)
      if (cmd.apply(*p)) ++cnt;
    if (cnt == 0) {
      fprintf(fp, "\n");
      return 0;
    }
    list_node **arr = new (std::nothrow) list_node *[cnt];
    list_node **temp = new (std::nothrow) list_node *[cnt];
    if (!arr || !temp) {
      delete[] arr;
      delete[] temp;
      fprintf(fp, "\n");
      return 0;
    }
    int idx = 0;
    for (list_node *p = group_heads[g]; p; p = p->group_next)
      if (cmd.apply(*p)) arr[idx++] = p;
    merge_sort(arr, temp, cnt, ob);
    for (int i = 0; i < cnt; ++i) arr[i]->print(cmd.get_order(), fp);
    delete[] arr;
    delete[] temp;
    fprintf(fp, "\n");
    return cnt;
  }
  if (cmd.get_op() == operation::land && has_name_eq && has_phone_eq) {
    stat_mixed++;
    list_node *name_bucket = name_index.get_bucket_head(cmd.get_name());
    list_node *phone_bucket = phone_index.get_bucket_head(cmd.get_phone());
    bool name_is_smaller = bucket_len(name_bucket, true, false) <=
                           bucket_len(phone_bucket, false, false);
    list_node *bucket = name_is_smaller ? name_bucket : phone_bucket;
    bool bucket_is_name = name_is_smaller;
    if (bucket_is_name)
      stat_name_index++;
    else
      stat_phone_index++;
    if (!need_sort) {
      for (list_node *p = bucket; p;
           p = next_in_bucket(p, bucket_is_name, false)) {
        if (cmd.apply(*p)) {
          p->print(cmd.get_order(), fp);
          ++found;
        }
      }
      fprintf(fp, "\n");
      return found;
    }
    int cnt = count_matches_in_bucket(bucket, bucket_is_name, false, cmd);
    if (cnt == 0) {
      fprintf(fp, "\n");
      return 0;
    }
    list_node **arr = new (std::nothrow) list_node *[cnt];
    list_node **temp = new (std::nothrow) list_node *[cnt];
    if (!arr || !temp) {
      delete[] arr;
      delete[] temp;
      fprintf(fp, "\n");
      return 0;
    }
    fill_matches_in_bucket(bucket, bucket_is_name, false, cmd, arr);
    merge_sort(arr, temp, cnt, ob);
    for (int i = 0; i < cnt; ++i) arr[i]->print(cmd.get_order(), fp);
    delete[] arr;
    delete[] temp;
    fprintf(fp, "\n");
    return cnt;
  }
  if (cmd.get_op() == operation::lor && has_name_eq && has_phone_eq) {
    stat_mixed++;
    stat_name_index++;
    stat_phone_index++;
    list_node *name_bucket = name_index.get_bucket_head(cmd.get_name());
    list_node *phone_bucket = phone_index.get_bucket_head(cmd.get_phone());
    bool name_is_smaller = bucket_len(name_bucket, true, false) <=
                           bucket_len(phone_bucket, false, false);
    list_node *a = name_is_smaller ? name_bucket : phone_bucket;
    bool a_is_name = name_is_smaller;
    list_node *b = name_is_smaller ? phone_bucket : name_bucket;
    bool b_is_name = !name_is_smaller;
    if (!need_sort) {
      for (list_node *p = a; p; p = next_in_bucket(p, a_is_name, false)) {
        if (cmd.apply(*p)) {
          p->print(cmd.get_order(), fp);
          ++found;
        }
      }
      for (list_node *p = b; p; p = next_in_bucket(p, b_is_name, false)) {
        if (!contains_ptr_in_bucket(a, a_is_name, false, p) && cmd.apply(*p)) {
          p->print(cmd.get_order(), fp);
          ++found;
        }
      }
      fprintf(fp, "\n");
      return found;
    }
    int total =
        count_union_matches(a, a_is_name, false, b, b_is_name, false, cmd);
    if (total == 0) {
      fprintf(fp, "\n");
      return 0;
    }
    list_node **arr = new (std::nothrow) list_node *[total];
    list_node **temp = new (std::nothrow) list_node *[total];
    if (!arr || !temp) {
      delete[] arr;
      delete[] temp;
      fprintf(fp, "\n");
      return 0;
    }
    fill_union_matches(a, a_is_name, false, b, b_is_name, false, cmd, arr);
    merge_sort(arr, temp, total, ob);
    for (int i = 0; i < total; ++i) arr[i]->print(cmd.get_order(), fp);
    delete[] arr;
    delete[] temp;
    fprintf(fp, "\n");
    return total;
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
    int cnt = count_matches_in_bucket(bucket, true, false, cmd);
    if (cnt == 0) {
      fprintf(fp, "\n");
      return 0;
    }
    list_node **arr = new (std::nothrow) list_node *[cnt];
    list_node **temp = new (std::nothrow) list_node *[cnt];
    if (!arr || !temp) {
      delete[] arr;
      delete[] temp;
      fprintf(fp, "\n");
      return 0;
    }
    fill_matches_in_bucket(bucket, true, false, cmd, arr);
    merge_sort(arr, temp, cnt, ob);
    for (int i = 0; i < cnt; ++i) arr[i]->print(cmd.get_order(), fp);
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
    int cnt = count_matches_in_bucket(bucket, false, false, cmd);
    if (cnt == 0) {
      fprintf(fp, "\n");
      return 0;
    }
    list_node **arr = new (std::nothrow) list_node *[cnt];
    list_node **temp = new (std::nothrow) list_node *[cnt];
    if (!arr || !temp) {
      delete[] arr;
      delete[] temp;
      fprintf(fp, "\n");
      return 0;
    }
    fill_matches_in_bucket(bucket, false, false, cmd, arr);
    merge_sort(arr, temp, cnt, ob);
    for (int i = 0; i < cnt; ++i) arr[i]->print(cmd.get_order(), fp);
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
  list_node **arr = new (std::nothrow) list_node *[cnt];
  list_node **temp = new (std::nothrow) list_node *[cnt];
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
  printf(
      "STAT: full_scan=%lld | name_index=%lld | phone_index=%lld | "
      "mixed=%lld\n",
      stat_full_scan, stat_name_index, stat_phone_index, stat_mixed);
}