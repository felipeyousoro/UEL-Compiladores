#include "program_declarations_node.h"

program_declarations_node *id_create_node(list_node *constant_declarations_list, list_node *global_declarations_list,
                                          list_node *function_declarations_list, list_node *string_declarations_list) {
    program_declarations_node *node = (program_declarations_node *) malloc(sizeof(program_declarations_node));
    node->constant_declarations_list = constant_declarations_list;
    node->global_declarations_list = global_declarations_list;
    node->function_declarations_list = function_declarations_list;
    node->string_declarations_list = string_declarations_list;
    return node;
}

int id_print_node(program_declarations_node *node, FILE *ast_file, int available_dot_id) {
    int node_dot_id = available_dot_id;
    available_dot_id++;

    fprintf(ast_file, "%i [label=\"INITIAL DECLARATIONS\", shape=oval];\n", node_dot_id);

    for (list_node *current = node->constant_declarations_list; current != NULL; current = current->next) {
        int dot_id = available_dot_id;
        fprintf(ast_file, "%i -> %i;\n", node_dot_id, dot_id);
        available_dot_id = cd_print_node(current->value, ast_file, dot_id);
    };

    for (list_node *current = node->global_declarations_list; current != NULL; current = current->next) {
        int dot_id = available_dot_id;
        fprintf(ast_file, "%i -> %i;\n", node_dot_id, dot_id);
        fprintf(ast_file, "%i [label=\"GLOBAL VARIABLE\" shape=oval];\n", dot_id);
        available_dot_id++;
        fprintf(ast_file, "%i -> %i;\n", dot_id, available_dot_id);
        available_dot_id = td_print_node(current->value, ast_file, available_dot_id);
    };

    for (list_node *current = node->function_declarations_list; current != NULL; current = current->next) {
        fprintf(ast_file, "%i -> %i;\n", node_dot_id, available_dot_id);
        available_dot_id = fd_print_node(current->value, ast_file, available_dot_id);
    };


    return available_dot_id;
}

/*
 * Constantes: geram um .word com o valor da constante
 * Variáveis globais:
 *          Inteiro: .word 0
 *          Caractere .byte 0
 *
 *          Matriz: .space * (tamanho da matriz)
 *              -> considerar inteiro como 4 bytes
 *              -> considerar caractere como 1 byte
 *
 *          Ponteiro: .word 0
 */

void pd_generate_code_data_section(program_declarations_node *node) {
    for (list_node *current = node->constant_declarations_list; current != NULL; current = current->next) {
        cd_generate_code_node(current->value);
    };

    for (list_node *current = node->global_declarations_list; current != NULL; current = current->next) {
        td_generate_code_data_section_node(current->value);
    };

    int string_label = 0;

    for (list_node *current = node->string_declarations_list; current != NULL; current = current->next) {
        printf("\t__string_%i: .asciiz \"%s\"\n", string_label, (char *) current->value);
        string_label++;
    };
}

void pd_generate_code_text_section(program_declarations_node *node) {
    for (list_node *current = node->function_declarations_list; current != NULL; current = current->next) {
        fd_generate_code_node(current->value);
    };
}