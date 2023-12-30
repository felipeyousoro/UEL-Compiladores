#include "expression_node.h"

expression_node* exp_create_node(int type, void* value) {
    expression_node* node = (expression_node*) malloc(sizeof(expression_node));
    node->type = type;
    node->value = value;
    return node;
}

int exp_print_node(expression_node *node, FILE *ast_file, int available_dot_id) {
    switch(node->type) {
        case EXP_PE:
            available_dot_id = pe_print_node(node->value, ast_file, available_dot_id);
            break;
        case EXP_PE_ARRAY:
            available_dot_id = pe_print_node(node->value, ast_file, available_dot_id);
            break;
        case EXP_BOP:
            available_dot_id = bop_print_node(node->value, ast_file, available_dot_id);
            break;
        case EXP_UOP:
            available_dot_id = uop_print_node(node->value, ast_file, available_dot_id);
            break;
        case EXP_TOP:
            available_dot_id = top_print_node(node->value, ast_file, available_dot_id);
            break;
        case EXP_FUNC_CALL:
            available_dot_id = fncc_print_node(node->value, ast_file, available_dot_id);
            break;
        default:
            available_dot_id++;
            break;
    }

    return available_dot_id;
}

int exp_evaluate_node(expression_node *node) {
    int value = 0;

    switch(node->type) {
        case EXP_PE:
            value = pe_evaluate_node(node->value);
            break;
//        case EXP_PE_ARRAY:
//            //value = pe_evaluate_node(node->value);
//            break;
//        case EXP_BOP:
//            //value = bop_evaluate_node(node->value);
//            break;
//        case EXP_UOP:
//            //value = uop_evaluate_node(node->value);
//            break;
//        case EXP_TOP:
//            //value = top_evaluate_node(node->value);
//            break;
//        case EXP_FUNC_CALL:
//            //value = fncc_evaluate_node(node->value);
//            break;
        default:
            break;
    }

    return value;
}

int exp_generate_code_node(expression_node *node) {
    int reg = 0;
    switch(node->type) {
        case EXP_PE:
            reg = pe_generate_code_node(node->value);
            break;
        case EXP_PE_ARRAY:
            reg = pe_generate_code_node(node->value);
            break;
        case EXP_BOP:
            reg = bop_generate_code_node(node->value);
            break;
        case EXP_UOP:
            reg = uop_generate_code_node(node->value);
            break;
        case EXP_TOP:
            top_generate_code_node(node->value);
            break;
        case EXP_FUNC_CALL:
            reg = fncc_generate_code_node(node->value);
            break;
        default:
            break;
    }

    return reg;
}

int exp_generate_code_global_offset_node(expression_node *node) {
    return pe_generate_code_global_offset_node(node->value);
}

char *exp_get_pe_id(expression_node *node) {
    return pe_get_id(node->value);
}