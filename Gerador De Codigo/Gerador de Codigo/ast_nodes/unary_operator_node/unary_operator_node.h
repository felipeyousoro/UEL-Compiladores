#ifndef UNARY_OPERATOR_NODE_H
#define UNARY_OPERATOR_NODE_H

#include <stdlib.h>

#include "../expression_node/expression_node.h"

enum {
    UOP_PREINC,
    UOP_PREDEC,
    UOP_POSTINC,
    UOP_POSTDEC,
    UOP_PLUS,
    UOP_MINUS,
    UOP_BITWISE_NOT,
    UOP_LOGICAL_NOT,
    UOP_POINTER
};

typedef struct {
    int type;
    int pointer_count;
    void *expr;
} unary_operator_node;

unary_operator_node *uop_create_node(int type, int pointer_count, void *expr);

int uop_print_node(unary_operator_node *node, FILE *ast_file, int available_dot_id);

int uop_generate_code_node(unary_operator_node *node);

#endif