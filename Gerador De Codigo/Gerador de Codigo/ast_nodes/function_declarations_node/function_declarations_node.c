#include "function_declarations_node.h"

function_internal_declarations_node *
fid_create_node(type_node *return_type, list_node *parameter_declarations_list, list_node *variable_declarations_list,
                list_node *commands_list) {
    function_internal_declarations_node *node = (function_internal_declarations_node *) malloc(
            sizeof(function_internal_declarations_node));
    node->return_type = return_type;
    node->parameter_declarations_list = parameter_declarations_list;
    node->variable_declarations_list = variable_declarations_list;
    node->commands_list = commands_list;
    return node;
}

function_declarations_node *
fd_create_node(char *id, function_internal_declarations_node *function_internal_declarations) {
    function_declarations_node *node = (function_declarations_node *) malloc(sizeof(function_declarations_node));
    node->id = id;
    node->function_internal_declarations = function_internal_declarations;
    return node;
}

int fd_print_node(function_declarations_node *node, FILE *ast_file, int available_dot_id) {
    int node_dot_id = available_dot_id;
    available_dot_id++;

    function_internal_declarations_node *func_int_dec = node->function_internal_declarations;

    fprintf(ast_file, "%i [label=\"FUNCTION\n", node_dot_id);
    fprintf(ast_file, "ID: %s\" shape=oval];\n", node->id);
    fprintf(ast_file, "%i -> %i;\n", node_dot_id, available_dot_id);
    available_dot_id = tn_print_node(func_int_dec->return_type, ast_file, available_dot_id);

    for (list_node *current = func_int_dec->parameter_declarations_list; current != NULL; current = current->next) {
        int dot_id = available_dot_id;
        fprintf(ast_file, "%i -> %i;\n", node_dot_id, available_dot_id);
        fprintf(ast_file, "%i [label=\"PARAMETER\", shape=oval];\n", available_dot_id);
        available_dot_id++;
        fprintf(ast_file, "%i -> %i;\n", dot_id, available_dot_id);
        available_dot_id = td_print_node(current->value, ast_file, available_dot_id);
    };

    for (list_node *current = func_int_dec->variable_declarations_list; current != NULL; current = current->next) {
        int dot_id = available_dot_id;
        fprintf(ast_file, "%i -> %i;\n", node_dot_id, dot_id);
        fprintf(ast_file, "%i [label=\"VARIABLE\", shape=oval];\n", dot_id);
        available_dot_id++;
        fprintf(ast_file, "%i -> %i;\n", dot_id, available_dot_id);
        available_dot_id = td_print_node(current->value, ast_file, available_dot_id);
    };

    for (list_node *current = func_int_dec->commands_list; current != NULL; current = current->next) {
        fprintf(ast_file, "%i -> %i;\n", node_dot_id, available_dot_id);
        available_dot_id = cl_print_node(current->value, ast_file, available_dot_id);
    };

    return available_dot_id;
}

/*
 * TODOS OS CONTEXTOS SERÃO SALVOS ANTES DE QUAISQUER CHAMADAS
 * DE FUNÇÕES, E SERÃO RESTAURADOS APÓS SUAS EXECUÇÕES
 */

void fd_generate_code_node(function_declarations_node *node) {
    function_internal_declarations_node *func_int_dec = node->function_internal_declarations;
    cm_new_scope();

    symbol_table *st = GET_CURRENT_CONTEXT();
    char **arguments = GET_CURRENT_ARGUMENTS_MAP();
    char **variables = GET_CURRENT_VARIABLES_MAP();

    // Salvando as variaveis
    for (list_node *current = func_int_dec->parameter_declarations_list; current != NULL; current = current->next) {
        symbol *s = st_create_symbol(SYMB_PARAM, td_get_id(current->value), current->value);
        st_push(st, s);
        cm_arguments_save_map(arguments, td_get_id(current->value));
    };

    for (list_node *current = func_int_dec->variable_declarations_list; current != NULL; current = current->next) {
        symbol *s = st_create_symbol(SYMB_VAR, td_get_id(current->value), current->value);
        st_push(st, s);
        cm_variables_save_map(variables, td_get_id(current->value));
    };

    printf("%s:\n", node->id);

    for (list_node *current = func_int_dec->commands_list; current != NULL; current = current->next) {
        cl_generate_code_node(current->value);
        //POSTINCS AND DECS
    };

    RETURN_RA();
    printf("\n");

    cm_exit_scope();
}