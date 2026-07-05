struct _node;
typedef struct _node
	{
		char * string;
		struct _node * next;
	} node;

void delete_list (node *head);
int read_list (const char *filename, node **head);
void print_list(node *head, int p);
int get_length(node * head);

