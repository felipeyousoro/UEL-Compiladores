#ifndef BINARY_OPERATOR_NODE_H
#define BINARY_OPERATOR_NODE_H

#include <stdio.h>
#include <stdlib.h>

#include "../../macros.h"
#include "../../context_manager.h"

#include "../expression_node/expression_node.h"

enum {
    BOP_PLUS,
    BOP_MINUS,
    BOP_MULTIPLY,
    BOP_DIV,
    BOP_REM,
    BOP_LESS_THAN,
    BOP_GREATER_THAN,
    BOP_LESS_EQUAL,
    BOP_GREATER_EQUAL,
    BOP_EQUAL,
    BOP_NOT_EQUAL,
    BOP_LOGICAL_AND,
    BOP_LOGICAL_OR,
    BOP_BITWISE_AND,
    BOP_BITWISE_OR,
    BOP_BITWISE_XOR,
    BOP_R_SHIFT,
    BOP_L_SHIFT,
    BOP_ASSIGN,
    BOP_PLUS_ASSIGN,
    BOP_MINUS_ASSIGN,
};

typedef struct binary_operator_body_node {
    void *left_expression;
    void *right_expression;
} binary_operator_body_node;

typedef struct binary_operator_node {
    int type;
    binary_operator_body_node *body;
} binary_operator_node;

binary_operator_body_node *bopb_create_node(void *left_expression, void *right_expression);

binary_operator_node *bop_create_node(int type, binary_operator_body_node *body);

int bop_print_node(binary_operator_node *node, FILE *ast_file, int available_dot_id);

int bop_generate_code_node(binary_operator_node *node);


#endif