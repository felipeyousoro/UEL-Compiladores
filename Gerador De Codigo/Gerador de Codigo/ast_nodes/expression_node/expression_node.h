#ifndef EXPRESSION_NODE_H
#define EXPRESSION_NODE_H

#include <stdio.h>
#include <stdlib.h>

#include "../../data_structures/list/list.h"
#include "../../data_structures/stack/stack.h"
#include "../../data_structures/symbol_table/symbol_table.h"

#include "../function_call_node/function_call_node.h"
#include "../unary_operator_node/unary_operator_node.h"
#include "../binary_operator_node/binary_operator_node.h"
#include "../ternary_operator_node/ternary_operator_node.h"
#include "../primary_expression_node/primary_expression_node.h"

enum {
    EXP_BOP,
    EXP_UOP,
    EXP_PE,
    EXP_PE_ARRAY,
    EXP_TOP,
    EXP_FUNC_CALL,
};

typedef struct expression_node {
    int type;
    void *value;
} expression_node;

expression_node* exp_create_node(int type, void* value);
int exp_print_node(expression_node *node, FILE *ast_file, int available_dot_id);

int exp_evaluate_node(expression_node *node);

int exp_generate_code_node(expression_node *node);

//Arg is a primary expression node
int exp_generate_code_global_offset_node(expression_node *node);
char *exp_get_pe_id(expression_node *node);

#endif