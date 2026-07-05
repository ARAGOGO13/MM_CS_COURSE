#include "functions.h"

list::~list() {
  clear();
}

void list::set_head(list_node* h) {
  head = h;
}

list_node* list::get_head() const {
  return head;
}

void list::clear() {
  while (head) {
    list_node* nx = head->get_next();
    delete head;
    head = nx;
  }
  head = nullptr;
}

int list::get_less_than_previous() const {
  if (!head) return 0;
  list_node* prev = head;
  list_node* cur = prev->get_next();
  int cnt = 0;
  while (cur) {
    if (*cur < *prev) ++cnt;
    prev = cur;
    cur = cur->get_next();
  }
  return cnt;
}

void list::sort_bubble() {
  if (!head || !head->get_next()) return;

  list_node* new_head = nullptr;

  while (head) {
    list_node* first = head;
    list_node* next = first->get_next();
    if (!next) {
      head = nullptr;
      first->set_next(new_head);
      new_head = first;
      break;
    }

    list_node* prev = nullptr;
    list_node* cur = head;

    while (cur && (next = cur->get_next())) {
      if (*cur > *next) {
        list_node* after = next->get_next();
        next->set_next(cur);
        cur->set_next(after);
        if (prev) prev->set_next(next);
        else head = next;
        prev = next;
      } else {
        prev = cur;
        cur = next;
      }
    }

    if (cur == head) {
      head = nullptr;
    } else {
      if (prev) prev->set_next(nullptr);
      else head = nullptr;
    }

    cur->set_next(new_head);
    new_head = cur;
  }

  head = new_head;
}

void list::sort_selection() {
  if (!head || !head->get_next()) return;

  list_node* out_head = nullptr;
  list_node* out_tail = nullptr;

  while (head) {
    list_node* min_prev = nullptr;
    list_node* min_node = head;

    list_node* prev = head;
    list_node* cur = head->get_next();
    while (cur) {
      if (*cur < *min_node) {
        min_prev = prev;
        min_node = cur;
      }
      prev = cur;
      cur = cur->get_next();
    }

    if (min_prev) min_prev->set_next(min_node->get_next());
    else head = min_node->get_next();

    min_node->set_next(nullptr);
    if (!out_head) {
      out_head = out_tail = min_node;
    } else {
      out_tail->set_next(min_node);
      out_tail = min_node;
    }
  }

  head = out_head;
}

void list::sort_insertion() {
  if (!head || !head->get_next()) return;

  list_node* sorted = nullptr;

  while (head) {
    list_node* cur = head;
    head = head->get_next();

    if (!sorted || *cur < *sorted) {
      cur->set_next(sorted);
      sorted = cur;
    } else {
      list_node* p = sorted;
      list_node* nx = p->get_next();
      while (nx && !(*cur < *nx)) {
        p = nx;
        nx = nx->get_next();
      }
      cur->set_next(nx);
      p->set_next(cur);
    }
  }

  head = sorted;
}

list_node* list::split_segment(list_node* start, int size) {
  if (!start) return nullptr;
  list_node* cur = start;
  for (int i = 1; i < size && cur->get_next(); ++i) cur = cur->get_next();
  list_node* rest = cur->get_next();
  cur->set_next(nullptr);
  return rest;
}

list_node* list::merge_two(list_node* a, list_node* b, list_node*& out_tail) {
  out_tail = nullptr;
  if (!a) {
    if (!b) return nullptr;
    list_node* t = b;
    while (t->get_next()) t = t->get_next();
    out_tail = t;
    return b;
  }
  if (!b) {
    list_node* t = a;
    while (t->get_next()) t = t->get_next();
    out_tail = t;
    return a;
  }

  list_node* headm = nullptr;
  list_node* tailm = nullptr;

  if (*a < *b) {
    headm = tailm = a;
    a = a->get_next();
  } else {
    headm = tailm = b;
    b = b->get_next();
  }
  tailm->set_next(nullptr);

  while (a && b) {
    if (*a < *b) {
      list_node* take = a;
      a = a->get_next();
      take->set_next(nullptr);
      tailm->set_next(take);
      tailm = take;
    } else {
      list_node* take = b;
      b = b->get_next();
      take->set_next(nullptr);
      tailm->set_next(take);
      tailm = take;
    }
  }

  if (a) {
    tailm->set_next(a);
    while (tailm->get_next()) tailm = tailm->get_next();
  } else if (b) {
    tailm->set_next(b);
    while (tailm->get_next()) tailm = tailm->get_next();
  }

  out_tail = tailm;
  return headm;
}

void list::sort_merge() {
  if (!head || !head->get_next()) return;

  while (true) {
    list_node* cur = head;
    list_node* new_head = nullptr;
    list_node* new_tail = nullptr;
    bool merged_any = false;

    while (cur) {
      list_node* A_head = cur;
      list_node* A_tail = cur;
      while (A_tail->get_next() && !(*A_tail->get_next() < *A_tail))
        A_tail = A_tail->get_next();
      list_node* rest = A_tail->get_next();
      A_tail->set_next(nullptr);

      if (!rest) {
        if (!new_head) {
          new_head = A_head;
          new_tail = A_tail;
        } else {
          new_tail->set_next(A_head);
          new_tail = A_tail;
        }
        break;
      }

      list_node* B_head = rest;
      list_node* B_tail = rest;
      while (B_tail->get_next() && !(*B_tail->get_next() < *B_tail))
        B_tail = B_tail->get_next();
      list_node* afterB = B_tail->get_next();
      B_tail->set_next(nullptr);

      list_node* merged_tail = nullptr;
      list_node* merged_head = merge_two(A_head, B_head, merged_tail);

      if (!new_head) {
        new_head = merged_head;
        new_tail = merged_tail;
      } else {
        new_tail->set_next(merged_head);
        new_tail = merged_tail;
      }

      merged_any = true;
      cur = afterB;
    }

    if (!merged_any) break;
    head = new_head;
  }
}

void list::append_node(list_node*& h, list_node*& t, list_node* node) {
  node->set_next(nullptr);
  if (!h) {
    h = t = node;
  } else {
    t->set_next(node);
    t = node;
  }
}

list_node*
list::quick_sort_rec(list_node* start, int len, list_node*& out_tail) {
  out_tail = nullptr;
  if (!start) return nullptr;
  if (len <= 1 || !start->get_next()) {
    list_node* tt = start;
    while (tt->get_next()) tt = tt->get_next();
    out_tail = tt;
    return start;
  }

  list_node* prev = nullptr;
  list_node* slow = start;
  list_node* fast = start;
  while (fast && fast->get_next()) {
    prev = slow;
    slow = slow->get_next();
    fast = fast->get_next()->get_next();
  }

  list_node* pivot = slow;
  if (prev) prev->set_next(pivot->get_next());
  else start = pivot->get_next();
  pivot->set_next(nullptr);

  list_node* Lh = nullptr;
  list_node* Lt = nullptr;
  list_node* Mh = nullptr;
  list_node* Mt = nullptr;
  list_node* Uh = nullptr;
  list_node* Ut = nullptr;
  int lenL = 0, lenU = 0;

  append_node(Mh, Mt, pivot);

  list_node* cur = start;
  while (cur) {
    list_node* nxt = cur->get_next();
    if (*cur < *pivot) {
      append_node(Lh, Lt, cur);
      ++lenL;
    } else if (*cur > *pivot) {
      append_node(Uh, Ut, cur);
      ++lenU;
    } else {
      append_node(Mh, Mt, cur);
    }
    cur = nxt;
  }

  list_node* left_head = nullptr;
  list_node* left_tail = nullptr;
  list_node* right_head = nullptr;
  list_node* right_tail = nullptr;

  if (lenL <= lenU) {
    if (Lh) left_head = quick_sort_rec(Lh, lenL, left_tail);
    if (Uh) right_head = quick_sort_rec(Uh, lenU, right_tail);
  } else {
    if (Uh) right_head = quick_sort_rec(Uh, lenU, right_tail);
    if (Lh) left_head = quick_sort_rec(Lh, lenL, left_tail);
  }

  list_node* new_head = nullptr;
  list_node* new_tail = nullptr;

  if (left_head) {
    new_head = left_head;
    new_tail = left_tail;
  }

  if (!new_head) {
    new_head = Mh;
    new_tail = Mt;
  } else {
    new_tail->set_next(Mh);
    new_tail = Mt;
  }

  if (right_head) {
    new_tail->set_next(right_head);
    new_tail = right_tail;
  }

  out_tail = new_tail;
  return new_head;
}

void list::sort_quick() {
  if (!head || !head->get_next()) return;
  int n = 0;
  for (list_node* p = head; p; p = p->get_next()) ++n;
  list_node* tail = nullptr;
  list_node* new_head = quick_sort_rec(head, n, tail);
  head = new_head;
}

io_status list::read(FILE* fp, unsigned int max_read) {
  clear();
  student buff;
  unsigned int count = 0;
  list_node* tail = nullptr;

  while (count < max_read) {
    io_status status = buff.read(fp);
    if (status != io_status::success) {
      if (count == 0 && status == io_status::eof) return io_status::eof;
      if (status == io_status::eof) break;
      clear();
      return status;
    }

    list_node* new_node = new list_node;
    if (!new_node) {
      clear();
      return io_status::memory;
    }

    *(student*)new_node = std::move(buff);
    new_node->set_next(nullptr);

    if (head == nullptr) {
      head = tail = new_node;
    } else {
      tail->set_next(new_node);
      tail = new_node;
    }

    ++count;
  }

  return io_status::success;
}

void list::print(unsigned int r, FILE* fp) const {
  (void)fp;

  const list_node* curr = head;
  unsigned int cnt = 0;
  while (curr != nullptr && cnt < r) {
    curr->print();
    curr = curr->get_next();
    ++cnt;
  }
}