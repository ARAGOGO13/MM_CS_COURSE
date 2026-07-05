#include "functions.h"

int count_of_max(list_node* head) {
  int cnt = 1;
  list_node* max_node = nullptr;
  list_node* curr = nullptr;

  if (head == nullptr) {
    return 0;
  }

  max_node = head;
  curr = head->get_next();

  while (curr != nullptr) {
    if (*curr > *max_node) {
      max_node = curr;
      cnt = 1;
    } else if (*curr == *max_node) {
      cnt++;
    }
    curr = curr->get_next();
  }

  return cnt;
}

int count_greater_than_prev(list_node* head) {
  int cnt = 0;
  list_node* prev = nullptr;
  list_node* curr = nullptr;

  if (head == nullptr) {
    return 0;
  }

  prev = head;
  curr = head->get_next();

  while (curr != nullptr) {
    if (*curr > *prev) {
      cnt++;
    }
    prev = curr;
    curr = curr->get_next();
  }

  return cnt;
}

int count_local_max(list_node* head) {
  int cnt = 0;
  list_node* prev = nullptr;
  list_node* curr = nullptr;
  list_node* next = nullptr;

  if (head == nullptr) { return 0; }
  if (head->get_next() == nullptr) { return 1; }

  prev = head;
  curr = head->get_next();
  next = curr->get_next();

  if (*prev >= *curr) { cnt++; }

  while (next != nullptr) {
    int is_loc_max = 1;
    if (*curr < *prev) is_loc_max = 0;
    if (*curr < *next) is_loc_max = 0;
    if (is_loc_max) { cnt++; }
    prev = curr;
    curr = next;
    next = next ? next->get_next() : nullptr;
  }

  if (*curr >= *prev) {
    cnt++;
  }

  return cnt;
}

int type_of_monotonicity(list_node* head) {
  if (head == nullptr || head->get_next() == nullptr) {
    return 4;
  }

  bool is_non_decreasing = true;
  bool is_non_increasing = true;
  bool is_constant = true;

  list_node* current = head;
  list_node* next = head->get_next();

  while (next != nullptr) {
    if (*current < *next) {
      is_non_increasing = false;
      is_constant = false;
    }
    if (*current > *next) {
      is_non_decreasing = false;
      is_constant = false;
    }

    current = next;
    next = next->get_next();
  }

  if (is_constant) {
    return 3;
  } else if (is_non_decreasing) {
    return 1;
  } else if (is_non_increasing) {
    return 2;
  } else {
    return 0;
  }
}

int max_strict_increasing_len(list_node* head) {
  int mx_dist = 0;
  int cnt = 1;
  list_node* prev = 0;
  list_node* curr = 0;

  if (head == 0) return 0;

  prev = head;
  curr = head->get_next();

  while (curr != 0) {
    if (*prev < *curr) {
      cnt++;
      if (cnt > mx_dist) mx_dist = cnt;
    } else cnt = 1;
    prev = curr;
    curr = curr->get_next();
  }

  return mx_dist;
}

int count_constant_segments(list_node* head) {
  int cnt = 0;

  list_node* prev = 0;
  list_node* curr = 0;

  if (head == 0 || head->get_next() == 0) return 0;

  prev = head;
  curr = head->get_next();

  while (curr != 0) {
    if (*prev == *curr) {
      cnt++;
      while (curr != 0 && *prev == *curr) {
        prev = curr;
        curr = curr->get_next();
      }
      if (curr == 0) break;
    }
    prev = curr;
    curr = curr->get_next();
  }

  return cnt;
}

int max_distance_between_constant_segments(list_node* head) {
  if (head == nullptr || head->get_next() == nullptr) {
    return 0;
  }

  int max_distance = 0;
  list_node* prev_segment_end = nullptr;
  list_node* curr = head;

  while (curr != nullptr) {
    list_node* segment_start = curr;
    list_node* segment_end = curr;

    while (segment_end->get_next() != nullptr &&
           *segment_end == *(segment_end->get_next())) {
      segment_end = segment_end->get_next();
    }

    bool is_constant_segment = (segment_start != segment_end);

    if (is_constant_segment) {
      if (prev_segment_end != nullptr) {
        list_node* between = prev_segment_end->get_next();
        int distance = 0;

        while (between != nullptr && between != segment_start) {
          distance++;
          between = between->get_next();
        }

        if (distance > max_distance) {
          max_distance = distance;
        }
      }
      prev_segment_end = segment_end;
    }

    curr = segment_end->get_next();
  }

  return max_distance;
}

int max_length_of_nonstrict_monotonicity(list_node* head) {
  if (head == nullptr) return 0;

  int max_len = 1;
  int inc_len = 1;
  int dec_len = 1;

  list_node* cur = head;
  while (cur->get_next() != nullptr) {
    list_node* nxt = cur->get_next();

    if (!(*nxt < *cur)) {
      ++inc_len;
    } else {
      inc_len = 1;
    }

    if (!(*cur < *nxt)) {
      ++dec_len;
    } else {
      dec_len = 1;
    }

    int cur_max = (inc_len > dec_len) ? inc_len : dec_len;
    if (cur_max > max_len) max_len = cur_max;

    cur = nxt;
  }

  return max_len;
}