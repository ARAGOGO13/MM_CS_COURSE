#include "io.h"
#include <cstdio>

void list2::clear() {
  list2_node* curr = head;
  while (curr != nullptr) {
    list2_node* next = curr->get_next();
    delete curr;
    curr = next;
  }
  head = tail = nullptr;
  length = 0;
}

void list2::add_node(list2_node* node) {
  if (tail == nullptr) {
    head = tail = node;
  } else {
    tail->set_next(node);
    node->set_prev(tail);
    tail = node;
  }
  length++;
}

void list2::remove_node(list2_node* node) {
  if (node->get_prev()) node->get_prev()->set_next(node->get_next());
  if (node->get_next()) node->get_next()->set_prev(node->get_prev());
  if (node == head) head = node->get_next();
  if (node == tail) tail = node->get_prev();
  length--;
}

io_status list2::read(FILE* fp, unsigned int max_read) {
  clear();
  student buff;
  unsigned int count = 0;

  while (count < max_read) {
    io_status status = buff.read(fp);
    if (status != io_status::success) {
      if (count == 0 && status == io_status::eof) return io_status::eof;
      if (status == io_status::eof) break;
      return status;
    }

    list2_node* new_node = new list2_node;
    if (!new_node) {
      clear();
      return io_status::memory;
    }

    *static_cast<student*>(new_node) = std::move(buff);
    add_node(new_node);
    count++;
  }

  return io_status::success;
}

void list2::print(unsigned int r, FILE* fp) const {
  list2_node* curr = head;
  unsigned int count = 0;
  while (curr != nullptr && count < r) {
    curr->print(fp);
    curr = curr->get_next();
    count++;
  }
}