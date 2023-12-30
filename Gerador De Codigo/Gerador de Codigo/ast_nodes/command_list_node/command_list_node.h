#ifndef COMMAND_LIST_NODE_H
#define COMMAND_LIST_NODE_H

#include <stdio.h>
#include <stdlib.h>

#include "../../data_structures/list/list.h"

#include "../../ast_nodes/expression_node/expression_node.h"

enum {
    CL_IF,
    CL_DO_WHILE,
    CL_WHILE,
    CL_FOR,
    CL_PRINTF,
    CL_SCANF,
    CL_EXIT,
    CL_RETURN,
    CL_EXPRESSION,
};

typedef struct command_list_node {
    int type;
    void *command;
} command_list_node;

typedef struct command_if_node {
    expression_node *condition;
    list_node *command_list;
    list_node *else_command_list;
} command_if_node;

typedef struct command_do_while_node {
    list_node *command_list;
    expression_node *stop_condition;
} command_do_while_node;


typedef struct command_while_node {
    list_node *command_list;
    expression_node *stop_condition;
} command_while_node;

typedef struct command_for_node {
    list_node *command_list;
    expression_node *initialization;
    expression_node *stop_condition;
    expression_node *increment;
} command_for_node;

typedef struct command_printf_node {
    char *string;
    list_node *expression_list;
} command_printf_node;

typedef struct command_scanf_node {
    char *string;
    char *id;
} command_scanf_node;

typedef struct command_exit_node {
    expression_node *expression;
} command_exit_node;

typedef struct command_return_node {
    expression_node *expression;
} command_return_node;

command_list_node *cl_create_node(int type, void *command);
command_if_node *cl_create_if_node(expression_node *condition, list_node *command_list, list_node *else_command_list);
command_do_while_node *cl_create_do_while_node(list_node *command_list, expression_node *stop_condition);
command_while_node *cl_create_while_node(expression_node *stop_condition, list_node *command_list);
command_for_node *cl_create_for_node(expression_node *initialization, expression_node *stop_condition, expression_node *increment, list_node *command_list);
command_printf_node *cl_create_printf_node(char *string, list_node *expression_list);
command_scanf_node *cl_create_scanf_node(char *string, char *id);
command_exit_node *cl_create_exit_node(expression_node *expression);
command_return_node *cl_create_return_node(expression_node *expression);

int cl_print_node(command_list_node *node, FILE *ast_file, int available_dot_id);

void cl_generate_code_node(command_list_node *node);

#endif