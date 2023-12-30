#ifndef PRIMARY_EXPRESSION_NODE_H
#define PRIMARY_EXPRESSION_NODE_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../../macros.h"
#include "../../context_manager.h"

#include "../../data_structures/list/list.h"
#include "../../data_structures/symbol_table/symbol_table.h"

#include "../expression_node/expression_node.h"
#include "../type_declaration_node/type_declaration_node.h"
#include "../constant_declarations_node/constant_declarations_node.h"

enum {
    PE_ID,
    PE_INT,
    PE_STRING,
    PE_CHAR
};

typedef struct primary_expression_node {
    int type;
    void *value;

    int has_array;
    list_node *array;
} primary_expression_node;

primary_expression_node* pe_create_node(int type, void* value);

void pe_set_array(primary_expression_node* pe, list_node* array);

int pe_print_node(primary_expression_node *node, FILE *ast_file, int available_dot_id);

int pe_evaluate_node(primary_expression_node *node);

int pe_generate_code_node(primary_expression_node *node);

int pe_generate_code_global_offset_node(primary_expression_node *node);

char *pe_get_id(primary_expression_node *node);

#endif