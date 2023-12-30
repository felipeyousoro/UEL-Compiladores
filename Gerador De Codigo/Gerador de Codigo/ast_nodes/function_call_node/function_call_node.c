#include "function_call_node.h"

function_call_node *fncc_create_node(char *id, list_node *expression_list) {
    function_call_node *node = malloc(sizeof(function_call_node));
    node->id = id;
    node->expression_list = expression_list;
    return node;
};

int fncc_print_node(function_call_node *node, FILE *ast_file, int available_dot_id) {
    int node_dot_id = available_dot_id;
    available_dot_id++;

    fprintf(ast_file, "%i [label=\"FUNCTION CALL\nID: %s\", shape=oval];\n", node_dot_id, node->id);

    for (list_node *current = node->expression_list; current != NULL; current = current->next) {
        fprintf(ast_file, "%i -> %i\n", node_dot_id, available_dot_id);
        available_dot_id = exp_print_node(current->value, ast_file, available_dot_id);
    }
    
    return available_dot_id;
}

int fncc_generate_code_node(function_call_node *node) {
    int registers_in_use = cm_save_context();

    int i = 0, reg_value;
    for(list_node *current = node->expression_list; current != NULL; current = current->next, i++) {
        reg_value = exp_generate_code_node(current->value);
        cm_free_register(reg_value);
        MIPS_MOVE(4 + i, reg_value);
    }

    JUMP_LINK(node->id);

    cm_restore_context(registers_in_use);

    return 2;
}