#ifndef SYMBOL_TABLE_H
#define SYMBOL_TABLE_H

#include <stdlib.h>
#include <string.h>

#include "../list/list.h"

enum {
    SYMB_CONST,
    SYMB_GLOBAL,
    SYMB_FUNC,
    SYMB_PARAM,
    SYMB_VAR,
};

typedef struct symbol {
    int type;
    char *name;
    void *value;
} symbol;

typedef struct symbol_table {
    list_node *symbols;
} symbol_table;

symbol* st_create_symbol(int type, char *name, void *value);
symbol_table *st_create_table();

void st_push(symbol_table *table, symbol *s);

symbol* st_lookup_symbol(symbol_table *table, char *name);

#endif