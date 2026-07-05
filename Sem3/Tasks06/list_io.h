#ifndef LIST_IO_H
#define LIST_IO_H

#include "io.h"

#define ERROR_OPEN (-1)
#define ERROR_READ (-2)
#define NOT_ENOUGH (-3)
#define SUCCESS 0

io_status read_list(list_node** head, FILE* fp);

void print_list(const list_node* head, int r);

void delete_list(list_node* head);


#endif