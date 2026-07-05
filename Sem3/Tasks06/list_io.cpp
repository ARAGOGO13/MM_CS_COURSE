#include "list_io.h"

io_status read_list(list_node** head, FILE* fp) {
  if (!head) return io_status::memory;
  *head = nullptr;
  list_node* tail = nullptr;
  student tmp;
  unsigned int count = 0;
  while (true) {
    io_status status = tmp.read(fp);
    if (status != io_status::success) {
      if (status == io_status::eof) {
        if (count == 0) return io_status::eof;
        break;
      } else {
        delete_list(*head);
        return status;
      }
    }
    list_node* node = new list_node;
    if (!node) {
      delete_list(*head);
      return io_status::memory;
    }
    *(student*)node = std::move(tmp);
    node->set_next(nullptr);
    if (*head == nullptr) {
      *head = tail = node;
    } else {
      tail->set_next(node);
      tail = node;
    }
    ++count;
  }
  return io_status::success;
}

void print_list(const list_node* head, int r) {
  int cnt = 0;
  const list_node* cur = head;
  while (cur != nullptr && cnt < r) {
    cur->print();
    cur = cur->get_next();
    ++cnt;
  }
}

void delete_list(list_node* head) {
  while (head) {
    list_node* nx = head->get_next();
    delete head;
    head = nx;
  }
}