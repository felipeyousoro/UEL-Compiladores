#include "command_list_node.h"

command_list_node *cl_create_node(int type, void *command) {
    command_list_node *node = (command_list_node *) malloc(sizeof(command_list_node));
    node->type = type;
    node->command = command;
    return node;
}

command_if_node *cl_create_if_node(expression_node *condition, list_node *command_list, list_node *else_command_list) {
    command_if_node *node = (command_if_node *) malloc(sizeof(command_if_node));
    node->condition = condition;
    node->command_list = command_list;
    node->else_command_list = else_command_list;
    return node;
}

command_do_while_node *cl_create_do_while_node(list_node *command_list, expression_node *stop_condition) {
    command_do_while_node *node = (command_do_while_node *) malloc(sizeof(command_do_while_node));
    node->command_list = command_list;
    node->stop_condition = stop_condition;
    return node;
}

command_while_node *cl_create_while_node(expression_node *stop_condition, list_node *command_list) {
    command_while_node *node = (command_while_node *) malloc(sizeof(command_while_node));
    node->stop_condition = stop_condition;
    node->command_list = command_list;
    return node;
}

command_for_node *
cl_create_for_node(expression_node *initialization, expression_node *stop_condition, expression_node *increment,
                   list_node *command_list) {
    command_for_node *node = (command_for_node *) malloc(sizeof(command_for_node));
    node->initialization = initialization;
    node->stop_condition = stop_condition;
    node->increment = increment;
    node->command_list = command_list;
    return node;
}

command_printf_node *cl_create_printf_node(char *string, list_node *expression_list) {
    command_printf_node *node = (command_printf_node *) malloc(sizeof(command_printf_node));
    node->string = string;
    node->expression_list = expression_list;
    return node;
}

command_scanf_node *cl_create_scanf_node(char *string, char *id) {
    command_scanf_node *node = (command_scanf_node *) malloc(sizeof(command_scanf_node));
    node->string = string;
    node->id = id;
    return node;
}

command_exit_node *cl_create_exit_node(expression_node *expression) {
    command_exit_node *node = (command_exit_node *) malloc(sizeof(command_exit_node));
    node->expression = expression;
    return node;
}

command_return_node *cl_create_return_node(expression_node *expression) {
    command_return_node *node = (command_return_node *) malloc(sizeof(command_return_node));
    node->expression = expression;
    return node;
}

static int print_if_node(command_if_node *node, FILE *ast_file, int available_dot_id) {
    int node_dot_id = available_dot_id;
    available_dot_id++;

    fprintf(ast_file, "%i [label=\"COMMAND\n", node_dot_id);
    fprintf(ast_file, "TYPE: IF\n");
    fprintf(ast_file, "\", shape=oval];\n");

    fprintf(ast_file, "%i -> %i;\n", node_dot_id, available_dot_id);
    available_dot_id = exp_print_node(node->condition, ast_file, available_dot_id);

    for (list_node *current = node->command_list; current != NULL; current = current->next) {
        fprintf(ast_file, "%i -> %i;\n", node_dot_id, available_dot_id);
        available_dot_id = cl_print_node(current->value, ast_file, available_dot_id);
    };

    for (list_node *current = node->else_command_list; current != NULL; current = current->next) {
        fprintf(ast_file, "%i -> %i;\n", node_dot_id, available_dot_id);
        available_dot_id = cl_print_node(current->value, ast_file, available_dot_id);
    };

    return available_dot_id;
}

static int print_do_while_node(command_do_while_node *node, FILE *ast_file, int available_dot_id) {
    int node_dot_id = available_dot_id;
    available_dot_id++;

    fprintf(ast_file, "%i [label=\"COMMAND\n", node_dot_id);
    fprintf(ast_file, "TYPE: DO-WHILE\n");
    fprintf(ast_file, "\", shape=oval];\n");

    fprintf(ast_file, "%i -> %i;\n", node_dot_id, available_dot_id);
    available_dot_id = exp_print_node(node->stop_condition, ast_file, available_dot_id);

    for (list_node *current = node->command_list; current != NULL; current = current->next) {
        fprintf(ast_file, "%i -> %i;\n", node_dot_id, available_dot_id);
        available_dot_id = cl_print_node(current->value, ast_file, available_dot_id);
    };

    return available_dot_id;
}

static int print_while_node(command_while_node *node, FILE *ast_file, int available_dot_id) {
    int node_dot_id = available_dot_id;
    available_dot_id++;

    fprintf(ast_file, "%i [label=\"COMMAND\n", node_dot_id);
    fprintf(ast_file, "TYPE: WHILE\n");
    fprintf(ast_file, "\", shape=oval];\n");

    fprintf(ast_file, "%i -> %i;\n", node_dot_id, available_dot_id);
    available_dot_id = exp_print_node(node->stop_condition, ast_file, available_dot_id);

    for (list_node *current = node->command_list; current != NULL; current = current->next) {
        fprintf(ast_file, "%i -> %i;\n", node_dot_id, available_dot_id);
        available_dot_id = cl_print_node(current->value, ast_file, available_dot_id);
    };

    return available_dot_id;
}

static int print_for_node(command_for_node *node, FILE *ast_file, int available_dot_id) {
    int node_dot_id = available_dot_id;
    available_dot_id++;

    fprintf(ast_file, "%i [label=\"COMMAND\n", node_dot_id);
    fprintf(ast_file, "TYPE: FOR\n");
    fprintf(ast_file, "\", shape=oval];\n");

    fprintf(ast_file, "%i -> %i;\n", node_dot_id, available_dot_id);
    available_dot_id = exp_print_node(node->initialization, ast_file, available_dot_id);

    fprintf(ast_file, "%i -> %i;\n", node_dot_id, available_dot_id);
    available_dot_id = exp_print_node(node->stop_condition, ast_file, available_dot_id);

    fprintf(ast_file, "%i -> %i;\n", node_dot_id, available_dot_id);
    available_dot_id = exp_print_node(node->increment, ast_file, available_dot_id);

    for (list_node *current = node->command_list; current != NULL; current = current->next) {
        fprintf(ast_file, "%i -> %i;\n", node_dot_id, available_dot_id);
        available_dot_id = cl_print_node(current->value, ast_file, available_dot_id);
    };

    return available_dot_id;
}

static int print_printf_node(command_printf_node *node, FILE *ast_file, int available_dot_id) {
    int node_dot_id = available_dot_id;
    available_dot_id++;

    fprintf(ast_file, "%i [label=\"COMMAND\n", node_dot_id);
    fprintf(ast_file, "TYPE: PRINTF\n");
    fprintf(ast_file, "\", shape=oval];\n");

    for (list_node *current = node->expression_list; current != NULL; current = current->next) {
        fprintf(ast_file, "%i -> %i;\n", node_dot_id, available_dot_id);
        available_dot_id = exp_print_node(current->value, ast_file, available_dot_id);
    };

    return available_dot_id;
}

static int print_scanf_node(command_scanf_node *node, FILE *ast_file, int available_dot_id) {
    int node_dot_id = available_dot_id;
    available_dot_id++;

    fprintf(ast_file, "%i [label=\"COMMAND\n", node_dot_id);
    fprintf(ast_file, "TYPE: SCANF\n");
    fprintf(ast_file, "\", shape=oval];\n");

    fprintf(ast_file, "%i -> %i;\n", node_dot_id, available_dot_id);
    fprintf(ast_file, "%i [label=\"%s\", shape=oval];\n", available_dot_id, node->id);
    available_dot_id++;

    return available_dot_id;
}

static int print_exit_node(command_exit_node *node, FILE *ast_file, int available_dot_id) {
    int node_dot_id = available_dot_id;
    available_dot_id++;

    fprintf(ast_file, "%i [label=\"COMMAND\n", node_dot_id);
    fprintf(ast_file, "TYPE: EXIT\n");
    fprintf(ast_file, "\", shape=oval];\n");

    fprintf(ast_file, "%i -> %i;\n", node_dot_id, available_dot_id);
    available_dot_id = exp_print_node(node->expression, ast_file, available_dot_id);

    return available_dot_id;
}

static int print_return_node(command_return_node *node, FILE *ast_file, int available_dot_id) {
    int node_dot_id = available_dot_id;
    available_dot_id++;

    fprintf(ast_file, "%i [label=\"COMMAND\n", node_dot_id);
    fprintf(ast_file, "TYPE: RETURN\n");
    fprintf(ast_file, "\", shape=oval];\n");

    if (node->expression != NULL) {
        fprintf(ast_file, "%i -> %i;\n", node_dot_id, available_dot_id);
        available_dot_id = exp_print_node(node->expression, ast_file, available_dot_id);
    }

    return available_dot_id;
}

int cl_print_node(command_list_node *node, FILE *ast_file, int available_dot_id) {
    switch (node->type) {
        case CL_IF:
            available_dot_id = print_if_node(node->command, ast_file, available_dot_id);
            break;
        case CL_DO_WHILE:
            available_dot_id = print_do_while_node(node->command, ast_file, available_dot_id);
            break;
        case CL_WHILE:
            available_dot_id = print_while_node(node->command, ast_file, available_dot_id);
            break;
        case CL_FOR:
            available_dot_id = print_for_node(node->command, ast_file, available_dot_id);
            break;
        case CL_PRINTF:
            available_dot_id = print_printf_node(node->command, ast_file, available_dot_id);
            break;
        case CL_SCANF:
            available_dot_id = print_scanf_node(node->command, ast_file, available_dot_id);
            break;
        case CL_EXIT:
            available_dot_id = print_exit_node(node->command, ast_file, available_dot_id);
            break;
        case CL_RETURN:
            available_dot_id = print_return_node(node->command, ast_file, available_dot_id);
            break;
        case CL_EXPRESSION:
            available_dot_id = exp_print_node(node->command, ast_file, available_dot_id);
            break;
        default:
            available_dot_id++;
            break;
    }

    return available_dot_id;
}

static void generate_code_if_node(command_if_node *node) {
    int label_j_false = GET_NEW_LABEL();
    int label_j_end = GET_NEW_LABEL();

    int condition_value = exp_generate_code_node(node->condition);
    printf("\tbeq $%i, $0, __label_if_false_%i\n", condition_value, label_j_false);
    cm_free_register(condition_value);

    for (list_node *current = node->command_list; current != NULL; current = current->next) {
        cl_generate_code_node(current->value);
    }
    printf("\tj __label_if_end_%i\n", label_j_end);

    printf("\t__label_if_false_%i:\n", label_j_false);
    for (list_node *current = node->else_command_list; current != NULL; current = current->next) {
        cl_generate_code_node(current->value);
    }

    printf("\t__label_if_end_%i:\n", label_j_end);

}

static void generate_code_do_while_node(command_do_while_node *node) {
    int label_j_begin = GET_NEW_LABEL();
    int label_j_test = GET_NEW_LABEL();

    printf("\t__label_do_while_begin_%i:\n", label_j_begin);
    for (list_node *current = node->command_list; current != NULL; current = current->next) {
        cl_generate_code_node(current->value);
    }

    printf("\t__label_do_while_test_%i:\n", label_j_test);
    int condition_value = cm_get_available_temp_register();
    int cond_expr = exp_generate_code_node(node->stop_condition);
    MIPS_ADD(condition_value, cond_expr, 0);
    cm_free_register(cond_expr);

    printf("\tbne $%i, $0, __label_do_while_begin_%i\n", condition_value, label_j_begin);
    cm_free_register(condition_value);
}

static void generate_code_while_node(command_while_node *node) {
    int label_j_begin = GET_NEW_LABEL();
    int label_j_test = GET_NEW_LABEL();

    int condition_value = cm_get_available_temp_register();

    printf("\tj __label_while_test_%i\n", label_j_test);

    printf("\t__label_while_begin_%i:\n", label_j_begin);
    for (list_node *current = node->command_list; current != NULL; current = current->next) {
        cl_generate_code_node(current->value);
    }

    printf("\t__label_while_test_%i:\n", label_j_test);
    int expr_value = exp_generate_code_node(node->stop_condition);
    MIPS_ADD(condition_value, expr_value, 0);
    cm_free_register(expr_value);

    printf("\tbne $%i, $0, __label_while_begin_%i\n", condition_value, label_j_begin);
    cm_free_register(condition_value);

}

static void generate_code_for_node(command_for_node *node) {
    int label_j_begin = GET_NEW_LABEL();
    int label_j_test = GET_NEW_LABEL();

    int condition_value = cm_get_available_temp_register();

    int init_reg = exp_generate_code_node(node->initialization);
    printf("j __label_for_test_%i\n", label_j_test);

    printf("__label_for_begin_%i:\n", label_j_begin);
    for (list_node *current = node->command_list; current != NULL; current = current->next) {
        cl_generate_code_node(current->value);
    }

    int inc_reg = exp_generate_code_node(node->increment);
    cm_free_register(inc_reg);

    printf("__label_for_test_%i:\n", label_j_test);
    int expr_value = exp_generate_code_node(node->stop_condition);
    MIPS_ADD(condition_value, expr_value, 0);
    cm_free_register(expr_value);

    printf("\tbne $%i, $0, __label_for_begin_%i\n", condition_value, label_j_begin);
    cm_free_register(condition_value);
    cm_free_register(init_reg);
}

static void generate_code_printf_node(command_printf_node *node) {
    int str_lbl = GET_NEXT_STRING_LABEL();
    printf("\tla $a0, __string_%i\n", str_lbl);
    CALL_PRINT_STRING(4);

    char *token = strtok(node->string, "%");
    for (list_node *current = node->expression_list; current != NULL; current = current->next) {
        token = strtok(NULL, "%");
        int register_value = exp_generate_code_node(current->value);
        switch (token[0]) {
            case 'd':
            CALL_PRINT_INT(register_value);
                break;
            case 'c':
            CALL_PRINT_CHAR(register_value);
                break;
            case 's':
            CALL_PRINT_STRING(register_value);
                break;
            default:
                break;
        }
        cm_free_register(register_value);
        int str_lbl = GET_NEXT_STRING_LABEL();
        printf("\tla $a0, __string_%i\n", str_lbl);
        CALL_PRINT_STRING(4);
    }

}

void generate_code_scanf_node(command_scanf_node *node) {
    char **map_args = GET_CURRENT_ARGUMENTS_MAP();
    char **map_vars = GET_CURRENT_VARIABLES_MAP();

    int reg = cm_arguments_get_register(map_args, node->id);
    int global = 0;
    if (reg == -1) {
        reg = cm_variables_get_register(map_vars, node->id);
    }
    if (reg == -1) {
        global = 1;
        reg = cm_get_available_temp_register();
    }

    char *token = strtok(node->string, "%");
    token = strtok(NULL, "%");
    switch (token[0]) {
        case 'd':
        CALL_READ_INT(reg);
            break;
        case 'c':
        CALL_READ_CHAR(reg);
            break;
        default:
            break;
    }

    if (global == 1) {
        MIPS_SW_LABEL(reg, 0, node->id);
        cm_free_register(reg);
    }

}

static void generate_code_exit_node(command_exit_node *node) {
    if (node->expression == NULL) {
        CALL_EXIT();
        return;
    }

    int exit_value = exp_generate_code_node(node->expression);
    CALL_EXIT_VALUE(exit_value);
    cm_free_register(exit_value);

}

static void generate_code_return_node(command_return_node *node) {
    if (node->expression != NULL) {
        int register_value = exp_generate_code_node(node->expression);
        MIPS_MOVE(2, register_value);
        cm_free_register(register_value);
    }

    RETURN_RA();

}

static void generate_code_expression_node(expression_node *node) {
    exp_generate_code_node(node);
}

void cl_generate_code_node(command_list_node *node) {
    switch (node->type) {
        case CL_IF:
            generate_code_if_node(node->command);
            break;
        case CL_DO_WHILE:
            generate_code_do_while_node(node->command);
            break;
        case CL_WHILE:
            generate_code_while_node(node->command);
            break;
        case CL_FOR:
            generate_code_for_node(node->command);
            break;
        case CL_PRINTF:
            generate_code_printf_node(node->command);
            break;
        case CL_SCANF:
            generate_code_scanf_node(node->command);
            break;
        case CL_EXIT:
            generate_code_exit_node(node->command);
            break;
        case CL_RETURN:
            generate_code_return_node(node->command);
            break;
        case CL_EXPRESSION:
            generate_code_expression_node(node->command);
            break;
        default:
            break;
    }

}
