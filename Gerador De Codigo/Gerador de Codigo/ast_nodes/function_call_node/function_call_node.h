#ifndef FUNCTION_CALL_NODE_H
#define FUNCTION_CALL_NODE_H

#include <stdio.h>
#include <stdlib.h>

#include "../../macros.h"
#include "../../context_manager.h"

#include "../../data_structures/list/list.h"

#include "../expression_node/expression_node.h"

typedef struct function_call_node {
    char *id;
    list_node *expression_list;
} function_call_node;

function_call_node *fncc_create_node(char *id, list_node *expression_list);

int fncc_print_node(function_call_node *node, FILE *ast_file, int available_dot_id);

int fncc_generate_code_node(function_call_node *node);

#endif