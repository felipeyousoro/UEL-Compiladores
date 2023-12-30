#ifndef LIST_H
#define LIST_H

#include <stdio.h>
#include <stdlib.h>

typedef struct list_node {
    void *value;
    struct list_node *next;
} list_node;

list_node *list_create_node(void *value);

void list_link_nodes(list_node *node1, list_node *node2);

int list_get_size(list_node *list);

void list_free(list_node *list);

int list_contains(list_node *list, void *value);

#endif


