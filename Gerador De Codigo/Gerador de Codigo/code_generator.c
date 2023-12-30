#include "code_generator.h"

static void generate_data_section(void *ast) {
    printf(".data\n");
    pd_generate_code_data_section(ast);
    printf("\n");
}

static void generate_text_section(void *ast) {
    printf(".text\n\n");
    printf("__gerador_start:\n");
    printf("\tjal main\n");
    CALL_EXIT();
    printf("\n");
    pd_generate_code_text_section(ast);
}

static void generate_macros_section() {
//    macro_rem();
//    printf("\n");
//    macro_exit();
//    printf("\n");
//    macro_sle();
//    printf("\n");
//    macro_seq();
//    printf("\n");
//    macro_print_int();
//    printf("\n");
//    macro_print_string();
//    printf("\n");
//    macro_print_char();
//    printf("\n");
//    macro_read_int();
//    printf("\n");
//    macro_read_char();
//    printf("\n");
//    macro_read_string();
//    printf("\n");
}

void cg_code_generate(void *ast) {
    cm_init_context_manager();

    generate_data_section(ast);
    printf("\n");

    generate_macros_section();
    printf("\n");

    generate_text_section(ast);
    printf("\n");
}


void start_dot(FILE *ast_file) {
    fprintf(ast_file, "digraph AST {\n");
    fprintf(ast_file, "node [shape=record];\n");
}

void end_dot(FILE *ast_file) {
    fprintf(ast_file, "}");
}

void cg_print_ast(void *ast, FILE *ast_file) {
    start_dot(ast_file);
    fprintf(ast_file, "0 [label=\"START\", shape=oval];\n");
    fprintf(ast_file, "0 -> 1;\n");
    id_print_node(ast, ast_file, 1);

    end_dot(ast_file);
}
