#ifndef LIST_H
#define LIST_H

#include <stdlib.h>

typedef struct list_node {
    void *value;
    struct list_node *next;
} list_node;

list_node *list_create_node(void *value);

void list_link_nodes(list_node *node1, list_node *node2);

int list_get_size(list_node *list);

#endif


