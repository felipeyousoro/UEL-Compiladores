%{
    #include <stdio.h>
    #include <stdlib.h>
    #include <string.h>

    extern int yylex();

    extern int yylineno;
    extern int yycolno;
    extern int yyleng;
    extern int comment_start_line;
    extern int comment_start_col;
    extern int last_token_line;
    extern int last_token_col;

    extern char *yytext;
    extern char nextline[1];

    void yyerror(char *s);

%}

%token VOID
%token INT
%token CHAR
%token RETURN
%token BREAK
%token SWITCH
%token CASE
%token DEFAULT
%token DO
%token WHILE
%token FOR
%token IF
%token ELSE
%token TYPEDEF
%token STRUCT
%token PLUS
%token MINUS
%token MULTIPLY
%token DIV
%token REMAINDER
%token INC
%token DEC
%token BITWISE_AND
%token BITWISE_OR
%token BITWISE_NOT
%token BITWISE_XOR
%token NOT
%token LOGICAL_AND
%token LOGICAL_OR
%token EQUAL
%token NOT_EQUAL
%token LESS_THAN
%token GREATER_THAN
%token LESS_EQUAL
%token GREATER_EQUAL
%token R_SHIFT
%token L_SHIFT
%token ASSIGN
%token ADD_ASSIGN
%token MINUS_ASSIGN
%token SEMICOLON
%token COMMA
%token COLON
%token L_PAREN
%token R_PAREN
%token L_CURLY_BRACKET
%token L_SQUARE_BRACKET
%token R_SQUARE_BRACKET
%token TERNARY_CONDITIONAL
%token NUMBER_SIGN
%token POINTER
%token PRINTF
%token SCANF
%token DEFINE
%token EXIT
%token IDENTIFIER
%token NUM_OCTAL
%token NUM_INTEGER
%token NUM_HEXA
%token STRING
%token CHARACTER
%token R_CURLY_BRACKET

%token LONG_IDENTIFIER
%token UNKNOWN_CHARACTER
%token UNTERMINATED_COMMENT

%token END_OF_FILE

%start programa

%%

programa: declaracoes programa_loop END_OF_FILE  { return 0; }
        | funcao programa_loop END_OF_FILE       { return 0; }
    ;

asterisco: MULTIPLY
         |
    ;

programa_loop: programa
             |
    ;

declaracoes: NUMBER_SIGN DEFINE IDENTIFIER expressao
           | declaracao_de_variaveis
           | declaracao_de_prototipos
    ;

funcao: tipo asterisco IDENTIFIER parametros L_CURLY_BRACKET funcao_declaracao_loop
    ;


funcao_declaracao_loop : declaracao_de_variaveis funcao_declaracao_loop
                       | comandos R_CURLY_BRACKET
    ;

declaracao_de_variaveis: tipo asterisco IDENTIFIER declaracao_de_variaveis_exp_loop ASSIGN expressao_de_atribuicao declaracao_de_variaveis_virg_loop SEMICOLON
                       | tipo asterisco IDENTIFIER declaracao_de_variaveis_exp_loop declaracao_de_variaveis_virg_loop SEMICOLON
    ;

declaracao_de_variaveis_exp_loop: L_SQUARE_BRACKET expressao R_SQUARE_BRACKET declaracao_de_variaveis_exp_loop
                                |
    ;

declaracao_de_variaveis_virg_loop: COMMA asterisco IDENTIFIER declaracao_de_variaveis_exp_loop ASSIGN expressao_de_atribuicao declaracao_de_variaveis_virg_loop
                                |  COMMA asterisco IDENTIFIER declaracao_de_variaveis_exp_loop declaracao_de_variaveis_virg_loop
                                |
    ;

declaracao_de_prototipos: tipo asterisco IDENTIFIER parametros SEMICOLON
    ;

parametros: L_PAREN R_PAREN
          | L_PAREN tipo asterisco IDENTIFIER parametros_exp_loop parametros_virg_loop R_PAREN
    ;

parametros_exp_loop: L_SQUARE_BRACKET expressao R_SQUARE_BRACKET parametros_exp_loop
                   |
    ;

parametros_virg_loop: COMMA tipo asterisco IDENTIFIER parametros_exp_loop parametros_virg_loop
                    |

tipo: INT
    | CHAR
    | VOID
    ;

bloco: L_CURLY_BRACKET comandos R_CURLY_BRACKET
    ;

comandos: lista_de_comandos comandos_loop
    ;

comandos_loop: lista_de_comandos comandos_loop
             |
    ;

lista_de_comandos: DO bloco WHILE L_PAREN expressao R_PAREN SEMICOLON
                 | IF L_PAREN expressao R_PAREN bloco
                 | IF L_PAREN expressao R_PAREN bloco ELSE bloco
                 | WHILE L_PAREN expressao R_PAREN bloco
                 | FOR L_PAREN lista_de_comandos_jump_exp_1
                 | PRINTF L_PAREN STRING R_PAREN SEMICOLON
                 | PRINTF L_PAREN STRING COMMA expressao R_PAREN SEMICOLON
                 | SCANF L_PAREN STRING COMMA BITWISE_AND IDENTIFIER R_PAREN SEMICOLON
                 | EXIT L_PAREN expressao R_PAREN SEMICOLON
                 | RETURN SEMICOLON
                 | RETURN expressao SEMICOLON
                 | expressao SEMICOLON
                 | SEMICOLON
                 | bloco
    ;

lista_de_comandos_jump_exp_1: expressao SEMICOLON lista_de_comandos_jump_exp_2
                            | SEMICOLON lista_de_comandos_jump_exp_2
    ;

lista_de_comandos_jump_exp_2: expressao SEMICOLON lista_de_comandos_jump_exp_3
                            | SEMICOLON lista_de_comandos_jump_exp_3
    ;

lista_de_comandos_jump_exp_3: expressao R_PAREN bloco
                            | R_PAREN bloco

expressao: expressao_de_atribuicao
         | expressao COMMA expressao_de_atribuicao
    ;

expressao_de_atribuicao: expressao_condicional
                       | expressao_unaria ASSIGN expressao_de_atribuicao
                       | expressao_condicional ADD_ASSIGN expressao_de_atribuicao
                       | expressao_condicional MINUS_ASSIGN expressao_de_atribuicao
    ;

expressao_condicional: expressao_or_logico
                     | expressao_or_logico TERNARY_CONDITIONAL expressao COLON expressao_condicional
    ;

expressao_or_logico: expressao_and_logico
                   | expressao_or_logico LOGICAL_OR expressao_and_logico
    ;

expressao_and_logico: expressao_or
                    | expressao_and_logico LOGICAL_AND expressao_or
    ;

expressao_or: expressao_xor
            | expressao_or BITWISE_OR expressao_xor
    ;

expressao_xor: expressao_and
             | expressao_xor BITWISE_XOR expressao_and
    ;

expressao_and: expressao_de_igualdade
             | expressao_and BITWISE_AND expressao_de_igualdade
    ;

expressao_de_igualdade: expressao_relacional
                      | expressao_de_igualdade EQUAL expressao_relacional
                      | expressao_de_igualdade NOT_EQUAL expressao_relacional
    ;

expressao_relacional: expressao_shift
                    | expressao_relacional LESS_THAN expressao_shift
                    | expressao_relacional GREATER_THAN expressao_shift
                    | expressao_relacional LESS_EQUAL expressao_shift
                    | expressao_relacional GREATER_EQUAL expressao_shift
    ;

expressao_shift: expressao_aditiva
               | expressao_shift L_SHIFT expressao_aditiva
               | expressao_shift R_SHIFT expressao_aditiva
    ;

expressao_aditiva: expressao_multiplicativa
                 | expressao_aditiva PLUS expressao_multiplicativa
                 | expressao_aditiva MINUS expressao_multiplicativa
    ;

expressao_multiplicativa: expressao_cast
                        | expressao_multiplicativa MULTIPLY expressao_cast
                        | expressao_multiplicativa DIV expressao_cast
                        | expressao_multiplicativa REMAINDER expressao_cast
    ;

expressao_cast: expressao_unaria
              | L_PAREN tipo asterisco R_PAREN expressao_cast
    ;

expressao_unaria: expressao_pos_fixa
                | INC expressao_unaria
                | DEC expressao_unaria
                | BITWISE_AND expressao_cast
                | MULTIPLY expressao_cast
                | PLUS expressao_cast
                | MINUS expressao_cast
                | NOT expressao_cast
                | BITWISE_NOT expressao_cast
    ;

expressao_pos_fixa: expressao_primaria
                  | expressao_pos_fixa L_SQUARE_BRACKET expressao R_SQUARE_BRACKET
                  | expressao_pos_fixa INC
                  | expressao_pos_fixa DEC
                  | expressao_pos_fixa L_PAREN R_PAREN
                  | expressao_pos_fixa L_PAREN expressao_de_atribuicao expressao_pos_fixa_loop R_PAREN
    ;

expressao_pos_fixa_loop: COMMA expressao_de_atribuicao
                       |
    ;

expressao_primaria: IDENTIFIER
                  | numero
                  | STRING
                  | CHARACTER
                  | L_PAREN expressao R_PAREN
    ;

numero: NUM_OCTAL
      | NUM_INTEGER
      | NUM_HEXA
    ;

%%

void get_line_content(char buffer[1024], int line_number) {
    fseek(stdin, 0, SEEK_SET);

    int i = 0;
    for (i; i < line_number; i++) {
        if(fgets(buffer, 1024, stdin) == NULL){
            buffer[0] = '\0';
        }
    }

}

void yyerror(char *s){

    if(yychar == LONG_IDENTIFIER){
        printf("%serror:lexical:%i:%i: identifier too long", nextline, yylineno, yycolno);
    }
    else if(yychar == UNKNOWN_CHARACTER){
        printf("%serror:lexical:%i:%i: %s", nextline, yylineno, yycolno, yytext);
    }
    else if(yychar == UNTERMINATED_COMMENT){
        printf("%serror:lexical:%i:%i: unterminated comment", nextline, comment_start_line, comment_start_col);
    }
    else if(yychar == END_OF_FILE){
        printf("%serror:syntax:%i:%i: expected declaration or statement at end of input\n", nextline, last_token_line, last_token_col);
        char line_buffer[1024];

        get_line_content(line_buffer, last_token_line);
        int last_index = strlen(line_buffer) - 1;
        if (line_buffer[last_index] == '\n') {
            line_buffer[last_index] = '\0';
        }
        
        printf("%s\n", line_buffer);

        int i;
        for(i = 0; i < last_token_col - 1; i++){
            printf(" ");
        }
        printf("^");
    }
    else{
        printf("%serror:syntax:%i:%i: %s\n", nextline, yylineno, yycolno - yyleng, yytext);
        char line_buffer[1024];

        get_line_content(line_buffer, yylineno);
        printf("%s", line_buffer);

        int i;
        for(i = 0; i < yycolno - yyleng - 1; i++){
            printf(" ");
        }
        printf("^");
    }

    exit(1);
}

int main(int argc, char** argv){
    yyparse();
    

    printf("%sSUCCESSFUL COMPILATION.", nextline);

    return 0;
}
