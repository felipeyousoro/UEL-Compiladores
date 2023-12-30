#include "unary_operator_node.h"

unary_operator_node *uop_create_node(int type, int pointer_count, void *expr) {
    unary_operator_node *node = malloc(sizeof(unary_operator_node));
    node->type = type;
    node->pointer_count = pointer_count;
    node->expr = expr;
    return node;
}

// enum {
//     UOP_PREINC,
//     UOP_PREDEC,
//     UOP_POSTINC,
//     UOP_POSTDEC,
//     UOP_PLUS,
//     UOP_MINUS,
//     UOP_BITWISE_NOT,
//     UOP_LOGICAL_NOT,
//     UOP_POINTER
// };

#define PRINT_UOP_CASE(bop_type) \
        fprintf(ast_file, "%i [label=\"%s\", shape=oval];\n", node_dot_id, bop_type); \
        fprintf(ast_file, "%i -> %i\n", node_dot_id, available_dot_id); \
        available_dot_id = exp_print_node(node->expr, ast_file, available_dot_id); \
        break;

int uop_print_node(unary_operator_node *node, FILE *ast_file, int available_dot_id) {
    int node_dot_id = available_dot_id;
    available_dot_id++;
    switch (node->type) {
        case UOP_PREINC:
            PRINT_UOP_CASE("++");
        case UOP_PREDEC:
            PRINT_UOP_CASE("--");
        case UOP_POSTINC:
            PRINT_UOP_CASE("++");
        case UOP_POSTDEC:
            PRINT_UOP_CASE("--");
        case UOP_PLUS:
            PRINT_UOP_CASE("+");
        case UOP_MINUS:
            PRINT_UOP_CASE("-");
        case UOP_BITWISE_NOT:
            PRINT_UOP_CASE("~");
        case UOP_LOGICAL_NOT:
            PRINT_UOP_CASE("!");
        case UOP_POINTER:
            PRINT_UOP_CASE("*");
        default:
            break;
    }
    
    return available_dot_id;
}

static int generate_code_uop_postinc(unary_operator_node *node) {
    symbol_table *st = GET_CURRENT_CONTEXT();

    symbol *s = NULL;

    char *id = exp_get_pe_id(node->expr);
    s = st_lookup_symbol(st, id);

    int return_reg = -1, orig_reg, value_reg, offset;

    char **map;
    switch(s->type) {
        case(SYMB_GLOBAL):
            return_reg = exp_generate_code_node(node->expr);
            value_reg = cm_get_available_temp_register();
            MIPS_ADD(value_reg, return_reg, 0);
            offset = exp_generate_code_global_offset_node(node->expr);
            MIPS_LW_LABEL(value_reg, offset, s->name);
            cm_free_register(value_reg);
            cm_free_register(offset);
            break;
        case(SYMB_VAR):
            map = GET_CURRENT_VARIABLES_MAP();
            orig_reg = cm_variables_get_register(map, s->name);
            return_reg = exp_generate_code_node(node->expr);
            MIPS_ADDI(orig_reg, orig_reg, 1);
            break;
        case (SYMB_PARAM):
            map = GET_CURRENT_ARGUMENTS_MAP();
            orig_reg = cm_arguments_get_register(map, s->name);
            return_reg = exp_generate_code_node(node->expr);
            MIPS_ADDI(orig_reg, orig_reg, 1);
            break;
        default:
            break;
    }

    return return_reg;
}


int uop_generate_code_node(unary_operator_node *node) {
    int r1 = -1, r2 = -1, return_reg = -1;
    symbol *global = NULL;

    switch (node->type) {
        case UOP_PREINC:
            break;
        case UOP_PREDEC:
            break;
        case UOP_POSTINC:
            return_reg = generate_code_uop_postinc(node);
            break;
        case UOP_POSTDEC:
            break;
        case UOP_PLUS:
            return_reg = exp_generate_code_node(node->expr);
            break;
        case UOP_MINUS:
            r1 = exp_generate_code_node(node->expr);
            return_reg = is_register_temporary(r1) ? r1 : cm_get_available_temp_register();
            MIPS_SUB(return_reg, 0, r1);
            break;
        case UOP_BITWISE_NOT:
            r1 = exp_generate_code_node(node->expr);
            return_reg = is_register_temporary(r1) ? r1 : cm_get_available_temp_register();
            MIPS_BITWISE_NOT(return_reg, r1);
            break;
        case UOP_LOGICAL_NOT:
            r1 = exp_generate_code_node(node->expr);
            return_reg = is_register_temporary(r1) ? r1 : cm_get_available_temp_register();
            MIPS_LOGICAL_NOT(return_reg, r1);
            break;
        case UOP_POINTER:
            break;
        default:
            break;
    }

    return return_reg;
}