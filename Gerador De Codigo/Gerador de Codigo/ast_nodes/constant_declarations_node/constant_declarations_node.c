#include "constant_declarations_node.h"

constant_declarations_node* cd_create_node(char* id, int value) {
    constant_declarations_node* node = (constant_declarations_node*) malloc(sizeof(constant_declarations_node));
    node->id = id;
    node->value = value;
    return node;
}

int cd_print_node(constant_declarations_node* node, FILE* ast_file, int available_dot_id) {
    int node_dot_id = available_dot_id;
    available_dot_id++;

    fprintf(ast_file, "%i [label=\"CONSTANT\nID: %s\nVALUE: %i\n\" shape=oval];\n", node_dot_id, node->id, node->value);

    return available_dot_id;
}

void cd_generate_code_node(constant_declarations_node* node) {
    symbol *s = st_create_symbol(SYMB_CONST, node->id, node);
    st_push(GET_CURRENT_CONTEXT(), s);

    printf("\t%s: .word %i\n", node->id, node->value);
}

int cd_get_value(constant_declarations_node* node) {
    return node->value;
}