#include "primary_expression_node.h"

primary_expression_node *pe_create_node(int type, void *value) {
    primary_expression_node *pe = malloc(sizeof(primary_expression_node));
    pe->type = type;
    pe->value = value;
    pe->has_array = 0;
    pe->array = NULL;
    return pe;
};

void pe_set_array(primary_expression_node *pe, list_node *array) {
    pe->has_array = 1;
    pe->array = array;
};

// enum {
//     PE_ID,
//     PE_INT,
//     PE_STRING,
//     PE_CHAR
// };


int pe_print_node(primary_expression_node *node, FILE *ast_file, int available_dot_id) {
    int node_dot_id = available_dot_id;
    available_dot_id++;

    fprintf(ast_file, "%i [label=\"PRIMARY EXPRESSION\n", node_dot_id);

    char type[10] = "";
    char value[100] = "";
    switch (node->type) {
        case PE_ID:
            strcpy(type, "ID");
            strcpy(value, (char *) node->value);
            break;
        case PE_INT:
            strcpy(type, "INT");
            sprintf(value, "%d", *((int *) node->value));
            break;
        case PE_STRING:
            strcpy(type, "STRING");
            strcpy(value, (char *) node->value);
            break;
        case PE_CHAR:
            strcpy(type, "CHAR");
            sprintf(value, "%c", *((char *) node->value));
            break;
    }
    fprintf(ast_file, "TYPE: %s", type);

    if (node->has_array == 1) {
        for (int i = 0; i < list_get_size(node->array); i++) {
            fprintf(ast_file, "[]");
        }
    }

    fprintf(ast_file, "\nVALUE: %s", value);

    fprintf(ast_file, "\", shape=oval];\n");

    if (node->has_array == 1) {
        for (list_node *current = node->array; current != NULL; current = current->next) {
            fprintf(ast_file, "%i -> %i;\n", node_dot_id, available_dot_id);
            available_dot_id = exp_print_node(current->value, ast_file, available_dot_id);
        }
    }

    return available_dot_id;
}

static int evaluate_id(primary_expression_node *node) {
    int value = 0;

    symbol_table *st = GET_CURRENT_CONTEXT();
    symbol *s = st_lookup_symbol(st, (char *) node->value);
    switch (s->type) {
        case SYMB_CONST:
            value = cd_get_value(s->value);
            break;
        default:
            break;
    }

    return value;
}

int pe_evaluate_node(primary_expression_node *node) {
    int value = 0;

    switch (node->type) {
        case PE_ID:
            value = evaluate_id(node);
            break;
        case PE_INT:
            value = *((int *) node->value);
            break;
//        case PE_STRING:
//            value = *((int *) node->value);
//            break;
        case PE_CHAR:
            value = *((char *) node->value);
            break;
        default:
            break;
    }

    return value;
}

static int get_offset(primary_expression_node *node) {
    int offset_reg = cm_get_available_temp_register();
    MIPS_ADDI(offset_reg, 0, 0);

    if(node->has_array == 0) {
        return offset_reg;
    }

    int size_reg = cm_get_available_temp_register();
    int size = 4;
    MIPS_ADDI(size_reg, 0, size);

//    switch (node->value->type) {
//        case BT_INT:
//            MIPS_ADDI(size_reg, 0, 4);
//            break;
//        case BT_CHAR:
//            MIPS_ADDI(size_reg, 0, 1);
//            break;
//        default:
//            MIPS_ADDI(size_reg, 0, 1);
//            break;
//    }

    printf("#\t\t\tCOM OFFSET\n");
    for (list_node *current = (list_node *) node->array; current != NULL; current = current->next) {
        int exp_reg = exp_generate_code_node(current->value);
        int index = cm_get_available_temp_register();
        MIPS_ADD(index, exp_reg, 0);
        MIPS_MUL(index, index, size_reg);
        MIPS_ADD(offset_reg, offset_reg, index);
        cm_free_register(index);
        cm_free_register(exp_reg);
    }

    cm_free_register(size_reg);

    return offset_reg;

}

static int generate_code_id(primary_expression_node *node) {
    int orig_reg = 0, offset = 0, return_reg = 0;

    symbol_table *st = GET_CURRENT_CONTEXT();
    symbol *s = st_lookup_symbol(st, (char *) node->value);

    if ( !s ) {
        return 0;
    }

    char **map;
    switch (s->type) {
        case SYMB_CONST:
            return_reg = cm_get_available_temp_register();
            SAVE_CONST(return_reg, cd_get_value(s->value));
            break;
        case SYMB_GLOBAL:
            return_reg = cm_get_available_temp_register();
            offset = get_offset(node);
            MIPS_LW_LABEL(return_reg, offset, s->name);
            cm_free_register(offset);
            break;
        case SYMB_PARAM:
            map = GET_CURRENT_ARGUMENTS_MAP();
            orig_reg = cm_arguments_get_register(map, s->name);
            return_reg = cm_get_available_temp_register();
            MIPS_ADD(return_reg, orig_reg, 0);
            break;
        case SYMB_VAR:
            map = GET_CURRENT_VARIABLES_MAP();
            orig_reg = cm_variables_get_register(map, s->name);
            return_reg = cm_get_available_temp_register();
            MIPS_ADD(return_reg, orig_reg, 0);
            break;
        default:
            break;
    }

    return return_reg;
}

int pe_generate_code_node(primary_expression_node *node) {
    int reg = 0;
    switch (node->type) {
        case PE_ID:
            reg = generate_code_id(node);
            break;
        case PE_INT:
            reg = cm_get_available_temp_register();
            SAVE_CONST(reg, *((int *) node->value));
            break;
        default:
            break;

    }

    return reg;
}

int pe_generate_code_global_offset_node(primary_expression_node *node) {
    return get_offset(node);
}

char *pe_get_id(primary_expression_node *node) {
    return (char *) node->value;
}