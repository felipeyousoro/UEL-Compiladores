#ifndef FUNCTION_DECLARATIONS_H
#define FUNCTION_DECLARATIONS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../../macros.h"
#include "../../context_manager.h"

#include "../../data_structures/list/list.h"

#include "../type_declaration_node/type_declaration_node.h"
#include "../command_list_node/command_list_node.h"

typedef struct function_internal_declarations_node {
    type_node *return_type;
    list_node *parameter_declarations_list;
    list_node *variable_declarations_list;
    list_node *commands_list;
} function_internal_declarations_node;

typedef struct function_declarations_node {
    char *id;
    function_internal_declarations_node *function_internal_declarations;
} function_declarations_node;

function_internal_declarations_node *fid_create_node(type_node *return_type, list_node *parameter_declarations_list, list_node *variable_declarations_list, list_node *commands_list);

function_declarations_node *fd_create_node(char *id, function_internal_declarations_node *function_internal_declarations);

int fd_print_node(function_declarations_node *node, FILE *ast_file, int available_dot_id);

void fd_generate_code_node(function_declarations_node *node);

#endif