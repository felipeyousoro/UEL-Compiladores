#include "ternary_operator_node.h"

ternary_operator_node *top_create_node(void *left_expression, void *middle_expression, void *right_expression){
    ternary_operator_node *node = (ternary_operator_node *) malloc(sizeof(ternary_operator_node));
    node->left_expression = left_expression;
    node->middle_expression = middle_expression;
    node->right_expression = right_expression;
    return node;
}

int top_print_node(ternary_operator_node *node, FILE *ast_file, int available_dot_id) {
    int node_dot_id = available_dot_id;
    available_dot_id++;
    
    fprintf(ast_file, "%i [label=\"?\", shape=oval];\n", node_dot_id);
    fprintf(ast_file, "%i -> %i\n", node_dot_id, available_dot_id);
    available_dot_id = exp_print_node(node->left_expression, ast_file, available_dot_id);
    fprintf(ast_file, "%i -> %i\n", node_dot_id, available_dot_id);
    available_dot_id = exp_print_node(node->middle_expression, ast_file, available_dot_id);
    fprintf(ast_file, "%i -> %i\n", node_dot_id, available_dot_id);
    available_dot_id = exp_print_node(node->right_expression, ast_file, available_dot_id);

    return available_dot_id;
}

void top_generate_code_node(ternary_operator_node *node) {


}