#ifndef PROGRAM_DECLARATIONS_NODE_H
#define PROGRAM_DECLARATIONS_NODE_H

#include <stdio.h>
#include <stdlib.h>

#include "../../data_structures/list/list.h"

#include "../type_declaration_node/type_declaration_node.h"
#include "../constant_declarations_node/constant_declarations_node.h"
#include "../function_declarations_node/function_declarations_node.h"

typedef struct program_declarations_node {
    list_node *constant_declarations_list;
    list_node *global_declarations_list;
    list_node *function_declarations_list;
    list_node *string_declarations_list;
} program_declarations_node;

program_declarations_node *id_create_node(list_node *constant_declarations_list, list_node *global_declarations_list, list_node *function_declarations_list, list_node *string_declarations_list);

int id_print_node(program_declarations_node *node, FILE *ast_file, int available_dot_id);

void pd_generate_code_data_section(program_declarations_node *node);
void pd_generate_code_text_section(program_declarations_node *node);

#endif