#ifndef CONSTANT_DECLARATIONS_NODE_H
#define CONSTANT_DECLARATIONS_NODE_H

#include <stdio.h>
#include <stdlib.h>

#include "../../context_manager.h"

typedef struct constant_declarations_node {
    char* id;
    int value;
} constant_declarations_node;

constant_declarations_node* cd_create_node(char* id, int value);

int cd_print_node(constant_declarations_node* node, FILE* ast_file, int available_dot_id);

void cd_generate_code_node(constant_declarations_node* node);

int cd_get_value(constant_declarations_node* node);

#endif