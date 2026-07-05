#ifndef LIST_IO_H
#define LIST_IO_H

#include "io.h"

#define ERROR_OPEN (-1)
#define ERROR_READ (-2)
#define NOT_ENOUGH (-3)
#define SUCCESS 0

void delete_list(list_node* head);

void print_list(const list_node* head, int r);

io_status read_list(list_node** p_head, FILE* fp);


#endif