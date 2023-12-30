#ifndef TERNARY_OPERATOR_NODE_H
#define TERNARY_OPERATOR_NODE_H

#include <stdlib.h>

#include "../expression_node/expression_node.h"

typedef struct ternary_operator_node {
    void *left_expression;
    void *middle_expression;
    void *right_expression;
} ternary_operator_node;


ternary_operator_node *top_create_node(void *left_expression, void *middle_expression, void *right_expression);

int top_print_node(ternary_operator_node *node, FILE *ast_file, int available_dot_id);

void top_generate_code_node(ternary_operator_node *node);

#endif