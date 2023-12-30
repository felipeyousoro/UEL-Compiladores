#include "symbol_table.h"


symbol *st_create_symbol(int type, char *name, void *value) {
    symbol *s = (symbol*) malloc(sizeof(symbol));
    s->type = type;
    s->name = name;
    s->value = value;
    return s;
}

symbol_table *st_create_table() {
    symbol_table *table = (symbol_table*) malloc(sizeof(symbol_table));
    table->symbols = NULL;
    return table;
}

void st_push(symbol_table *table, symbol *s) {
    list_node *node = (list_node*) malloc(sizeof(list_node));
    node->value = s;
    list_link_nodes(node, table->symbols);
    table->symbols = node;
}

symbol *st_lookup_symbol(symbol_table *table, char *name) {
    for (list_node *current = table->symbols; current != NULL; current = current->next) {
        symbol *s = (symbol*) current->value;
        if (strcmp(s->name, name) == 0) {
            return s;
        }
    }

    return NULL;
}