#include "io.h"
#include <cstdio>

void list2::shift_right(int k) {
  if (length <= 1) return;

  k = k % (int)length;
  if (k < 0) k += length;
  if (k == 0) return;

  list2_node* new_tail = head;
  for (size_t i = 0; i < length - k - 1; i++) {
    new_tail = new_tail->get_next();
  }

  list2_node* new_head = new_tail->get_next();

  new_tail->set_next(nullptr);
  new_head->set_prev(nullptr);

  tail->set_next(head);
  head->set_prev(tail);

  head = new_head;
  tail = new_tail;
}

void list2::remove_greater_than_previous_k(int k) {
  if (k <= 0 || head == nullptr) return;

  list2_node* curr = tail;

  while (curr != nullptr) {
    list2_node* prev_of_curr = curr->get_prev();

    int i = 0;
    list2_node* prev_k_node = curr->get_prev();

    for (; (prev_k_node != nullptr) && (i < k);
           prev_k_node = prev_k_node->get_prev()) {
      if (*curr > *prev_k_node)
        break;
      i++;
    }

    if ((prev_k_node != nullptr) && (i < k)) {
      list2_node* next_node = curr->get_next();

      if (prev_of_curr != nullptr) {
        prev_of_curr->set_next(next_node);
      } else {
        head = next_node;
      }

      if (next_node != nullptr) {
        next_node->set_prev(prev_of_curr);
      } else {
        tail = prev_of_curr;
      }

      delete curr;
      if (length > 0) length--;
      curr = prev_of_curr;
    } else {
      curr = prev_of_curr;
    }
  }
}


void list2::remove_greater_than_next_k(int k) {
  if (k <= 0 || length == 0) return;

  list2_node* curr = head;

  while (curr != nullptr) {
    list2_node* next = curr->get_next();
    bool should_remove = false;

    list2_node* nxt = curr->get_next();
    for (int i = 0; i < k && nxt != nullptr; i++) {
      if (*curr > *nxt) {
        should_remove = true;
        break;
      }
      nxt = nxt->get_next();
    }

    if (should_remove) {
      remove_node(curr);
      delete curr;
    }

    curr = next;
  }
}

void list2::remove_k_local_maxima(int k) {
  if (k <= 0 || (int)length < 2 * k + 1) return;

  list2_node* curr = head;
  unsigned int index = 0;

  while (curr != nullptr) {
    list2_node* next = curr->get_next();

    bool is_k_local_max = true;

    list2_node* left = curr->get_prev();
    for (int i = 0; i < k && left != nullptr; i++) {
      if (*curr < *left) {
        is_k_local_max = false;
        break;
      }
      left = left->get_prev();
    }
    if (left == nullptr && (int)index < k) is_k_local_max = false;

    if (is_k_local_max) {
      list2_node* right = curr->get_next();
      for (int i = 0; i < k && right != nullptr; i++) {
        if (*curr < *right) {
          is_k_local_max = false;
          break;
        }
        right = right->get_next();
      }
      if (right == nullptr && index > length - k - 1) is_k_local_max = false;
    }

    if (is_k_local_max) {
      remove_node(curr);
      delete curr;
    }

    curr = next;
    index++;
  }
}

void list2::remove_constant_segments_longer_than_k(int k) {
  if (k <= 0) return;

  list2_node* curr = head;

  while (curr != nullptr) {
    list2_node* segment_start = curr;
    int segment_length = 1;

    while (curr->get_next() != nullptr && *curr == *(curr->get_next())) {
      curr = curr->get_next();
      segment_length++;
    }

    list2_node* segment_end = curr;
    list2_node* next_after = segment_end->get_next();

    if (segment_length > k) {
      list2_node* to_remove = segment_start;
      while (to_remove != next_after) {
        list2_node* nxt = to_remove->get_next();
        remove_node(to_remove);
        delete to_remove;
        to_remove = nxt;
      }
    }

    curr = next_after;
  }
}

void list2::remove_non_increasing_segments_longer_than_k(int k) {
  if (k <= 0) return;

  list2_node* curr = head;

  while (curr != nullptr) {
    list2_node* segment_start = curr;
    int segment_length = 1;

    while (curr->get_next() != nullptr && *curr >= *(curr->get_next())) {
      curr = curr->get_next();
      segment_length++;
    }

    list2_node* segment_end = curr;
    list2_node* next_after = segment_end->get_next();

    if (segment_length > k) {
      list2_node* prev_before = segment_start->get_prev();

      if (prev_before != nullptr) {
        prev_before->set_next(next_after);
      } else {
        head = next_after;
      }

      if (next_after != nullptr) {
        next_after->set_prev(prev_before);
      } else {
        tail = prev_before;
      }

      list2_node* to_remove = segment_start;
      while (to_remove != next_after) {
        list2_node* nxt = to_remove->get_next();
        delete to_remove;
        if (length > 0) length--;
        to_remove = nxt;
      }
    }

    curr = next_after;
  }
}

void list2::remove_between_constant_segments_longer_than_k(int k) {
  if (k <= 0) return;

  list2_node* curr = head;
  list2_node* prev_segment_end = nullptr;

  while (curr != nullptr) {
    list2_node* segment_start = curr;
    int segment_length = 1;

    while (curr->get_next() != nullptr && *curr == *(curr->get_next())) {
      curr = curr->get_next();
      segment_length++;
    }

    if (segment_length > k) {
      if (prev_segment_end != nullptr) {
        list2_node* between_start = prev_segment_end->get_next();
        list2_node* between_end = segment_start->get_prev();

        if (between_start != nullptr && between_end != nullptr &&
            between_start != segment_start) {
          list2_node* to_remove = between_start;
          while (to_remove != segment_start) {
            list2_node* next = to_remove->get_next();
            remove_node(to_remove);
            delete to_remove;
            to_remove = next;
          }
        }
      }
      prev_segment_end = curr;
    }

    curr = curr->get_next();
  }
}

void list2::remove_between_non_decreasing_segments_longer_than_k(int k) {
  if (k <= 0 || head == nullptr || head->get_next() == nullptr) return;

  int len = 0;
  int flag = 0;

  list2_node *curr = nullptr,
      *next = nullptr,
      *start_area = nullptr;

  for (curr = head, next = curr->get_next(); next != nullptr;
       curr = next, next = curr->get_next()) {
    if (*curr <= *next) {
      if (len == 0) {
        len = 2;
        start_area = curr;
      } else
        len++;
    } else {
      if (len) {
        if (flag > (k + 1)) {
          list2_node* end_of_removable = start_area->get_prev();
          int nodes_to_delete = flag - 1;

          list2_node* start_of_removable = end_of_removable;
          for (int i = 0; i < nodes_to_delete - 1; i++) {
            if (start_of_removable == nullptr) break; // Защита
            start_of_removable = start_of_removable->get_prev();
          }

          list2_node* prev_of_removable = start_of_removable->get_prev();

          if (prev_of_removable != nullptr) {
            prev_of_removable->set_next(start_area);
          } else {
            head = start_area;
          }
          start_area->set_prev(prev_of_removable);

          list2_node* to_delete = start_of_removable;
          while (to_delete != start_area) {
            list2_node* next_to_delete = to_delete->get_next();
            delete to_delete;
            if (length > 0) length--;
            to_delete = next_to_delete;
          }
        }

        flag = 1;
        len = 0;
      } else {
        flag += (flag > 0) ? 1 : 0;
      }
    }
  }

  if (len > 0 && (flag > (k + 1))) {
    list2_node* end_of_removable = start_area->get_prev();
    int nodes_to_delete = flag - 1;

    list2_node* start_of_removable = end_of_removable;
    for (int i = 0; i < nodes_to_delete - 1; i++) {
      if (start_of_removable == nullptr) break;
      start_of_removable = start_of_removable->get_prev();
    }

    list2_node* prev_of_removable = (start_of_removable != nullptr)
                                      ? start_of_removable->get_prev()
                                      : nullptr;

    if (prev_of_removable != nullptr) {
      prev_of_removable->set_next(start_area);
    } else {
      head = start_area;
    }
    start_area->set_prev(prev_of_removable);

    list2_node* to_delete = start_of_removable;
    while (to_delete != start_area) {
      list2_node* next_to_delete = to_delete->get_next();
      delete to_delete;
      if (length > 0) length--;
      to_delete = next_to_delete;
    }
  }

  len = 0;
  flag = 0;
}