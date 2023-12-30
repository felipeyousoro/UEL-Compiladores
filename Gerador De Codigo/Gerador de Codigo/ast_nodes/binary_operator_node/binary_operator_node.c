#include "binary_operator_node.h"

binary_operator_body_node *bopb_create_node(void *left_expression, void *right_expression) {
    binary_operator_body_node *node = malloc(sizeof(binary_operator_body_node));
    node->left_expression = left_expression;
    node->right_expression = right_expression;
    return node;
}

binary_operator_node *bop_create_node(int type, binary_operator_body_node *body){
    binary_operator_node *node = malloc(sizeof(binary_operator_node));
    node->type = type;
    node->body = body;
    return node;
}

#define PRINT_BOP_CASE(bop_type) \
        fprintf(ast_file, "%i [label=\"%s\", shape=oval];\n", node_dot_id, bop_type); \
        fprintf(ast_file, "%i -> %i\n", node_dot_id, available_dot_id); \
        available_dot_id = exp_print_node(node->body->left_expression, ast_file, available_dot_id); \
        fprintf(ast_file, "%i -> %i\n", node_dot_id, available_dot_id); \
        available_dot_id = exp_print_node(node->body->right_expression, ast_file, available_dot_id); \
        break;

int bop_print_node(binary_operator_node *node, FILE *ast_file, int available_dot_id) {
    int node_dot_id = available_dot_id;
    available_dot_id++;
    switch (node->type) {
        case BOP_PLUS:
            PRINT_BOP_CASE("+");
        case BOP_MINUS:
            PRINT_BOP_CASE("-");
        case BOP_MULTIPLY:
            PRINT_BOP_CASE("*");
        case BOP_DIV:
            PRINT_BOP_CASE("/");
        case BOP_REM:
            PRINT_BOP_CASE("%%");
        case BOP_LESS_THAN:
            PRINT_BOP_CASE("<");
        case BOP_GREATER_THAN:
            PRINT_BOP_CASE(">");
        case BOP_LESS_EQUAL:
            PRINT_BOP_CASE("<=");
        case BOP_GREATER_EQUAL:
            PRINT_BOP_CASE(">=");
        case BOP_EQUAL:
            PRINT_BOP_CASE("==");
        case BOP_NOT_EQUAL:
            PRINT_BOP_CASE("!=");
        case BOP_LOGICAL_AND:
            PRINT_BOP_CASE("&&");
        case BOP_LOGICAL_OR:
            PRINT_BOP_CASE("||");
        case BOP_BITWISE_AND:
            PRINT_BOP_CASE("&");
        case BOP_BITWISE_OR:
            PRINT_BOP_CASE("|");
        case BOP_BITWISE_XOR:
            PRINT_BOP_CASE("^");
        case BOP_R_SHIFT:
            PRINT_BOP_CASE(">>");
        case BOP_L_SHIFT:
            PRINT_BOP_CASE("<<");
        case BOP_ASSIGN:
            PRINT_BOP_CASE("=");
        case BOP_PLUS_ASSIGN:
            PRINT_BOP_CASE("+=");
        case BOP_MINUS_ASSIGN:  
            PRINT_BOP_CASE("-=");
        default:
            break;
    }
    
    return available_dot_id;
}

#undef PRINT_BOP_CASE

/*
 * Tipos de entradas para expressões:
 *  - Variáveis     -> usar o registrador s0-s7 ou a0-a3
 *  - Constantes    -> trocar pelo valor da constante e depois carregar no registrador t0-t7
 *  - Globais       -> aplicar lw para carregar o valor no registrador t0-t7
 *  - Funções       -> salvar contexto, aplicar jal, restaurar contexto
 *                         e o retorno está em v0 (possivelmente será necessário
 *                         salvar em um registrador temporário)
 *  - Resultado de expressões
 *                  -> salvar o resultado em um registrador temporário
 */

static int generate_plus_code(binary_operator_node *node) {
    int result_reg = cm_get_available_temp_register();
    int r1 = exp_generate_code_node(node->body->left_expression);
    int r2 = exp_generate_code_node(node->body->right_expression);

    MIPS_ADD(result_reg, r1, r2);

    cm_free_register(r1);
    cm_free_register(r2);

    return result_reg;
}

#define LOAD_BOP_REGISTERS \
    r1 = exp_generate_code_node(node->body->left_expression); \
    r2 = exp_generate_code_node(node->body->right_expression);\
    return_reg = is_register_temporary(r1) ? r1 : is_register_temporary(r2) ? r2 : cm_get_available_temp_register();

#define FREE_BOP_REGISTERS \
    return_reg == r1 ? cm_free_register(r2) : return_reg == r2 ? cm_free_register(r1) : (cm_free_register(r1), cm_free_register(r2));


static void generate_code_bop_assign(binary_operator_node *node) {
    symbol_table *st = GET_CURRENT_CONTEXT();

    symbol *s = NULL;

    char *id = exp_get_pe_id(node->body->left_expression);
    s = st_lookup_symbol(st, id);

    int left_reg = -1;
    int right_reg = exp_generate_code_node(node->body->right_expression);
    int orig_reg = -1;

    char **map;
    switch(s->type) {
        case(SYMB_GLOBAL):
            left_reg = exp_generate_code_global_offset_node(node->body->left_expression);
            MIPS_SW_LABEL(right_reg, left_reg, s->name);
            break;
        case(SYMB_VAR):
            map = GET_CURRENT_VARIABLES_MAP();
            orig_reg = cm_variables_get_register(map, s->name);
            MIPS_ADD(orig_reg, 0, right_reg);
            break;
        case (SYMB_PARAM):
            map = GET_CURRENT_ARGUMENTS_MAP();
            orig_reg = cm_variables_get_register(map, s->name);
            MIPS_ADD(orig_reg, 0, right_reg);
            break;
        default:
            break;
    }

    cm_free_register(left_reg);
    cm_free_register(right_reg);

};

static void generate_code_bop_plus_assign(binary_operator_node *node) {
    symbol_table *st = GET_CURRENT_CONTEXT();

    symbol *s = NULL;

    char *id = exp_get_pe_id(node->body->left_expression);
    s = st_lookup_symbol(st, id);

    int left_reg = -1, tmp_reg = -1;
    int right_reg = exp_generate_code_node(node->body->right_expression);
    int orig_reg = -1;

    char **map;
    switch(s->type) {
        case(SYMB_GLOBAL):
            left_reg = exp_generate_code_global_offset_node(node->body->left_expression);
            tmp_reg = cm_get_available_temp_register();
            MIPS_LW_LABEL(tmp_reg, left_reg, s->name);
            MIPS_ADD(tmp_reg, tmp_reg, right_reg);
            MIPS_SW_LABEL(tmp_reg, left_reg, s->name);
            break;
        case(SYMB_VAR):
            map = GET_CURRENT_VARIABLES_MAP();
            orig_reg = cm_variables_get_register(map, s->name);
            left_reg = exp_generate_code_node(node->body->left_expression);
            MIPS_ADD(orig_reg, left_reg, right_reg);
            break;
        case (SYMB_PARAM):
            map = GET_CURRENT_ARGUMENTS_MAP();
            orig_reg = cm_variables_get_register(map, s->name);
            left_reg = exp_generate_code_node(node->body->left_expression);
            MIPS_ADD(orig_reg, left_reg, right_reg);
            break;
        default:
            break;
    }

    cm_free_register(left_reg);
    cm_free_register(right_reg);
    cm_free_register(tmp_reg);
    cm_free_register(orig_reg);
};

static void generate_code_bop_minus_assign(binary_operator_node *node) {
    symbol_table *st = GET_CURRENT_CONTEXT();

    symbol *s = NULL;

    char *id = exp_get_pe_id(node->body->left_expression);
    s = st_lookup_symbol(st, id);

    int left_reg = -1, tmp_reg = -1;
    int right_reg = exp_generate_code_node(node->body->right_expression);
    int orig_reg = -1;

    char **map;
    switch(s->type) {
        case(SYMB_GLOBAL):
            left_reg = exp_generate_code_global_offset_node(node->body->left_expression);
            tmp_reg = cm_get_available_temp_register();
            MIPS_LW_LABEL(tmp_reg, left_reg, s->name);
            MIPS_SUB(tmp_reg, tmp_reg, right_reg);
            MIPS_SW_LABEL(tmp_reg, left_reg, s->name);
            break;
        case(SYMB_VAR):
            map = GET_CURRENT_VARIABLES_MAP();
            orig_reg = cm_variables_get_register(map, s->name);
            left_reg = exp_generate_code_node(node->body->left_expression);
            MIPS_SUB(orig_reg, left_reg, right_reg);
            break;
        case (SYMB_PARAM):
            map = GET_CURRENT_ARGUMENTS_MAP();
            orig_reg = cm_variables_get_register(map, s->name);
            left_reg = exp_generate_code_node(node->body->left_expression);
            MIPS_SUB(orig_reg, left_reg, right_reg);
            break;
        default:
            break;
    }

    cm_free_register(left_reg);
    cm_free_register(right_reg);
    cm_free_register(tmp_reg);
    cm_free_register(orig_reg);
};

int bop_generate_code_node(binary_operator_node *node) {
    int r1 = -1, r2 = -1, return_reg = -1;
    symbol *global = NULL;

    switch (node->type) {
        case BOP_PLUS:
            LOAD_BOP_REGISTERS;
            MIPS_ADD(return_reg, r1, r2);
            FREE_BOP_REGISTERS;
            break;
        case BOP_MINUS:
            LOAD_BOP_REGISTERS;
            MIPS_SUB(return_reg, r1, r2);
            FREE_BOP_REGISTERS;
            break;
        case BOP_MULTIPLY:
            LOAD_BOP_REGISTERS;
            MIPS_MUL(return_reg, r1, r2);
            FREE_BOP_REGISTERS;
            break;
        case BOP_DIV:
            LOAD_BOP_REGISTERS;
            MIPS_DIV(return_reg, r1, r2);
            FREE_BOP_REGISTERS;
            break;
        case BOP_REM:
            LOAD_BOP_REGISTERS;
            MIPS_REM(return_reg, r1, r2);
            FREE_BOP_REGISTERS;
            break;
        case BOP_LESS_THAN:
            LOAD_BOP_REGISTERS;
            MIPS_SLT(return_reg, r1, r2);
            FREE_BOP_REGISTERS;
            break;
        case BOP_GREATER_THAN:
            LOAD_BOP_REGISTERS;
            MIPS_SGT(return_reg, r1, r2);
            FREE_BOP_REGISTERS;
            break;
        case BOP_LESS_EQUAL:
            LOAD_BOP_REGISTERS;
            MIPS_SLE(return_reg, r1, r2);
            FREE_BOP_REGISTERS;
            break;
        case BOP_GREATER_EQUAL:
            LOAD_BOP_REGISTERS;
            MIPS_SGE(return_reg, r1, r2);
            FREE_BOP_REGISTERS;
            break;
        case BOP_EQUAL:
            LOAD_BOP_REGISTERS;
            MIPS_SEQ(return_reg, r1, r2);
            FREE_BOP_REGISTERS;
            break;
        case BOP_NOT_EQUAL:
            break;
        case BOP_LOGICAL_AND:
            LOAD_BOP_REGISTERS;
            MIPS_LOGICAL_AND(return_reg, r1, r2);
            FREE_BOP_REGISTERS;
            break;
        case BOP_LOGICAL_OR:
            LOAD_BOP_REGISTERS;
            MIPS_LOGICAL_OR(return_reg, r1, r2);
            FREE_BOP_REGISTERS;
            break;
        case BOP_BITWISE_AND:
            LOAD_BOP_REGISTERS;
            MIPS_BITWISE_AND(return_reg, r1, r2);
            FREE_BOP_REGISTERS;
            break;
        case BOP_BITWISE_OR:
            LOAD_BOP_REGISTERS;
            MIPS_BITWISE_OR(return_reg, r1, r2);
            FREE_BOP_REGISTERS;
            break;
        case BOP_BITWISE_XOR:
            LOAD_BOP_REGISTERS;
            MIPS_BITWISE_XOR(return_reg, r1, r2);
            FREE_BOP_REGISTERS;
            break;
        case BOP_R_SHIFT:
            LOAD_BOP_REGISTERS;
            MIPS_SRL(return_reg, r1, r2);
            FREE_BOP_REGISTERS;
            break;
        case BOP_L_SHIFT:
            LOAD_BOP_REGISTERS;
            MIPS_SLL(return_reg, r1, r2);
            FREE_BOP_REGISTERS;
            break;
        case BOP_ASSIGN:
            generate_code_bop_assign(node);
            break;
        case BOP_PLUS_ASSIGN:
            generate_code_bop_plus_assign(node);
            break;
        case BOP_MINUS_ASSIGN:
            generate_code_bop_minus_assign(node);
            break;
        default:
            break;
    }

    return return_reg;
}