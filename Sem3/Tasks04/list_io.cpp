#include "list_io.h"
#include <cstdio>

void delete_list(list_node* head) {
  list_node *curr, *next;
  for (curr = head; curr; curr = next) {
    next = curr->get_next();
    delete curr;
  }
}

void print_list(const list_node* head, int r) {
  const list_node* curr = 0;
  for (curr = head; curr != nullptr && r--; curr = curr->get_next()) {
    curr->print();
  }
}

io_status read_list(list_node **p_head, FILE* fp)
{
        list_node buf;
        io_status ret;
        list_node *head, *tail, *curr;
        if ((ret = buf.read(fp)) != io_status::success)
        {
                return ret;
        }
        head = new list_node;
        if (head == nullptr)
        {
                return io_status::memory;
        }
        *head = (list_node&&) buf;
        curr = head;
        while(buf.read(fp) == io_status::success)
        {
                tail = new list_node;
                if(tail == nullptr)
                {
                        delete_list(head);
                        return io_status::memory;
                }
                *tail = (list_node&&)buf;
                curr -> set_next(tail);
                curr = tail;
        }
        if(!feof(fp))
        {
                delete_list(head);
                return io_status::format;
        }
        *p_head = head;
        return io_status::success;
}
