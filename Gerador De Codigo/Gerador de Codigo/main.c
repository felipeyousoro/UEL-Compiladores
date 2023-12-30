#include <stdio.h>

#include "sintatico.tab.h"
#include "code_generator.h"

extern void *ast_return;

int main(int argc, char **argv) {
    yyparse();

    for (int i = 0; i < argc; i++) {
        if (strcmp(argv[i], "-g") == 0) {
            FILE *ast_file = fopen(argv[i + 1], "w");
            cg_print_ast(ast_return, ast_file);
            fclose(ast_file);
            break;
        }
    }

    cg_code_generate(ast_return);

    return 0;
}
