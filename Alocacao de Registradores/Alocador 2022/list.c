#include "list.h"

list_node *list_create_node(void *value) {
    list_node *node = malloc(sizeof(list_node));
    node->value = value;
    node->next = NULL;
    return node;
}

void list_link_nodes(list_node *node1, list_node *node2) {
    node1->next = node2;
}

int list_get_size(list_node *list) {
    int size = 0;
    while (list != NULL) {
        size++;
        list = list->next;
    }
    return size;
}

void list_free(list_node *list) {
    list_node *next;
    while (list != NULL) {
        next = list->next;
        free(list);
        list = next;
    }
}

int list_contains(list_node *list, void *value) {
    while (list != NULL) {
        if (list->value == value) {
            return 1;
        }
        list = list->next;
    }
    return 0;
}