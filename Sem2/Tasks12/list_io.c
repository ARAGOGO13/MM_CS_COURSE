#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "list_io.h"

void delete_list (node *head)
{
	node *curr, *next;
	for (curr = head; curr; curr = next)
		{
			next = curr -> next;
			free(curr -> string);
			free(curr);
		}
}

int read_list (const char *filename, node **head)
{
	char buffer[1234];
	node *current = 0;

	FILE *file = fopen(filename, "r");
	if (!file) { return -1; }


    *head = 0;

    while (fgets(buffer, sizeof(buffer), file)) 
	    {
	        size_t len = strlen(buffer);
	        char *str = 0;
	        node *new_node = 0;
	        if (len > 0 && buffer[len - 1] == '\n') 
		        {
		            buffer[len - 1] = 0;
		            len--;
		        }

	        str = malloc(len + 1);
	        if (!str) 
		        {
		            fclose(file);
		            delete_list(*head);
		            return -3;
		        }
	        strcpy(str, buffer);

	        new_node = malloc(sizeof(node));
	        if (!new_node) 
		        {
		            free(str);
		            fclose(file);
		            delete_list(*head);
		            return -3;
		        }
	        new_node->string = str;
	        new_node->next = 0;

	        if (!*head) 
		        {
		            *head = new_node;
		            current = *head;
		        } 
	        else 
		        {
		            current->next = new_node;
		            current = new_node;
		        }
	    }

    if (ferror(file)) 
	    {
	        fclose(file);
	        delete_list(*head);
	        *head = 0;
	        return -2;
	    }

    fclose(file);
    return 0;
}

void print_list(node *head, int p) 
{
    int count = 0;
    node *current = head;
    while (current && count < p) 
	    {
	        printf("%s\n", current->string);
	        current = current->next;
	        count++;
	    }
}

int get_length(node * head)
{
	node * curr;
	int i = 0;
	for (curr = head; curr; curr = curr->next) i++;
	return i;
}
