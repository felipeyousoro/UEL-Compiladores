#ifndef TYPE_DECLARATION_NODE_H
#define TYPE_DECLARATION_NODE_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../../context_manager.h"

#include "../../data_structures/list/list.h"

#include "../expression_node/expression_node.h"

enum {
    BT_INT,
    BT_VOID,
    BT_CHAR
};

enum {
    TE_NULL,
    TE_POINTERS,
    TE_ARRAYS
};

typedef struct type_extension_node {
    int type;
    void *value;
} type_extension_node;

typedef struct type_node {
    int type;
    type_extension_node *extension;
} type_node;

typedef struct type_declaration_node {
    char *id;
    type_node *type;
} type_declaration_node;

type_extension_node *ten_create_node(int type, void *value);
type_node *tn_create_node(int type, type_extension_node *extension);
type_declaration_node *td_create_node(char *id, type_node *type);

int td_print_node(type_declaration_node *node, FILE *ast_file, int available_dot_id);
int tn_print_node(type_node *node, FILE *ast_file, int available_dot_id);

void td_generate_code_data_section_node(type_declaration_node *node);
int td_generate_code_global_offset_node(type_declaration_node *node);

char *td_get_id(type_declaration_node *node);

#endif