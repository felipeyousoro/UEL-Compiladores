%{
    #include <stdio.h>
    #include <stdlib.h>
    #include <string.h>

    #include "data_structures/list/list.h"

    #include "ast_nodes/constant_declarations_node/constant_declarations_node.h"
    #include "ast_nodes/primary_expression_node/primary_expression_node.h"
    #include "ast_nodes/expression_node/expression_node.h"
    #include "ast_nodes/unary_operator_node/unary_operator_node.h"
    #include "ast_nodes/binary_operator_node/binary_operator_node.h"
    #include "ast_nodes/ternary_operator_node/ternary_operator_node.h"
    #include "ast_nodes/function_call_node/function_call_node.h"
    #include "ast_nodes/command_list_node/command_list_node.h"
    #include "ast_nodes/type_declaration_node/type_declaration_node.h"
    #include "ast_nodes/function_declarations_node/function_declarations_node.h"
    #include "ast_nodes/program_declarations_node/program_declarations_node.h"

    extern int yylex();

    void yyerror(char *s){
        printf("*----------------------*\n\tERRO\n*----------------------*\n");
    }

    #define CREATE_STRING(value) char *new_str = (char*)malloc(sizeof(char) * (strlen(value) + 1)); strcpy(new_str, value);
    #define CREATE_CHAR(value) char *new_char = (char*)malloc(sizeof(char) + 1); new_char[0] = value; new_char[1] = '\0';
    #define CREATE_INT(value) int *new_int = (int*)malloc(sizeof(int)); *new_int = value;

    void *ast_return = NULL;

    list_node *strings = NULL;

    list_node *split_string(char *string) {
        list_node *list = NULL;
        list_node *current = NULL;
        char *str = string + 1;
        str[strlen(str) - 1] = '\0';

        char *token;
        if(str[0] == '%') {
            str--;
            str[0] = 'X';
            token = strtok(str, "%");
            token++;
        }
        else {
            token = strtok(str, "%");
        }

        while (token != NULL) {
            CREATE_STRING(token);
            list_node *new_node = list_create_node(new_str);
            if (list == NULL) {
                list = new_node;
                current = list;
            } else {
                current->next = list_create_node(new_str + 1);
                current = current->next;
            }
            token = strtok(NULL, "%");
        }

        return list;
    }

    void insert_string_in_global_list(char *string) {

        list_node *splited_string = split_string(string);
        if (strings == NULL) {
            strings = splited_string;
        } else {
            list_node *current = strings;
            while (current->next != NULL) {
                current = current->next;
            }
            current->next = splited_string;
        }
    }

%}

%union{
    int integer;
    char character;
    char* string;
    void* ast_node;
}

%token START
%token FUNCTION
%token RETURN_TYPE
%token PARAMETER
%token RETURN 
%token END_FUNCTION
%token PRINTF
%token SCANF
%token CONSTANT
%token GLOBAL_VARIABLE
%token VALUE
%token VARIABLE 
%token TYPE
%token IF
%token ID
%token INT 
%token CHAR 
%token VOID
%token BITWISE_AND
%token BITWISE_OR
%token BITWISE_XOR
%token CHARACTER
%token STRING
%token NUM_INTEGER
%token PLUS
%token MINUS
%token MULTIPLY
%token DIV
%token REMAINDER
%token LESS_THAN
%token GREATER_THAN
%token LESS_EQUAL
%token GREATER_EQUAL
%token SEMICOLON
%token COMMA
%token L_PAREN
%token R_PAREN
%token L_SQUARE_BRACKET
%token R_SQUARE_BRACKET
%token POINTER
%token NUMBER_SIGN
%token L_CURLY_BRACKET
%token R_CURLY_BRACKET
%token COLON 
%token TERNARY_CONDITIONAL
%token PLUS_ASSIGN
%token MINUS_ASSIGN
%token ASSIGN
%token R_SHIFT
%token L_SHIFT
%token EQUAL
%token NOT_EQUAL
%token LOGICAL_OR
%token LOGICAL_AND
%token LOGICAL_NOT
%token BITWISE_NOT
%token DEC
%token INC
%token NEWLINE 

%token WHILE
%token DO_WHILE
%token FOR
%token EXIT

%start program

%type <integer> NUM_INTEGER
%type <string> ID
%type <string> STRING
%type <character> CHARACTER 

// %type <ast> constant
// %type <ast> global_variable

%type <ast_node> expression
%type <ast_node> primary_expression
%type <ast_node> primary_expression_array
%type <ast_node> ternary_operator
%type <ast_node> function_call
%type <ast_node> unary_operator
%type <ast_node> unary_operator_body
%type <ast_node> binary_operator
%type <ast_node> binary_operator_body

%type <ast_node> has_one_or_more_expressions
%type <ast_node> has_many_expressions

%type <integer> has_one_or_more_newlines
%type <integer> has_many_newlines 
%type <integer> has_one_or_more_pointers
%type <integer> has_many_pointers

%type <ast_node> array
%type <ast_node> has_many_arrays

%type <integer> basic_type
%type <ast_node> type_extension
%type <ast_node> type

%type <ast_node> value_adjust
%type <ast_node> stop_condition
%type <ast_node> conditional_expression
%type <ast_node> return_internal_expression
%type <ast_node> for_initialization

%type <ast_node> command_list_then
%type <ast_node> command_list_else
%type <ast_node> else_statement
%type <ast_node> command_list
%type <ast_node> has_one_or_more_commands
%type <ast_node> has_many_commands

%type <ast_node> type_declaration
%type <ast_node> has_many_function_declarations
%type <ast_node> variable_declarations
%type <ast_node> parameter_declarations
%type <ast_node> return_type_declaration
%type <ast_node> function_internal_declarations
%type <ast_node> function_declarations
%type <ast_node> global_declarations
%type <ast_node> constant_declarations
%type <ast_node> program_declarations
%type <ast_node> start
%type <ast_node> program

%%
program: start                  { ast_return = $1; return 0; }
    ;

start: 
    START has_one_or_more_newlines program_declarations
                                { $$ = $3; }
    ;

/*  
    DECLARATIONS 
    DECLARATIONS 
    DECLARATIONS 
                    */

program_declarations:
    constant_declarations global_declarations function_declarations
                                { program_declarations_node *idn = id_create_node($1, $2, $3, strings); $$ = idn; }
    ;

constant_declarations:
    CONSTANT COLON ID VALUE COLON NUM_INTEGER has_one_or_more_newlines constant_declarations
                                { constant_declarations_node *cdn = cd_create_node($3, $6);
                                    list_node *ln = list_create_node(cdn); list_link_nodes(ln, $8); $$ = ln; }
    |                           { $$ = NULL; }
    ;

global_declarations:
    GLOBAL_VARIABLE COLON type_declaration has_one_or_more_newlines global_declarations
                                { list_node *ln = list_create_node($3); list_link_nodes(ln, $5); $$ = ln; }
    |                           { $$ = NULL; }
    ;

function_declarations: 
    FUNCTION COLON ID NEWLINE function_internal_declarations END_FUNCTION has_many_function_declarations
                                { function_declarations_node *fdn = fd_create_node($3, $5); 
                                    list_node *ln = list_create_node(fdn); list_link_nodes(ln, $7); $$ = ln; }
    ;

function_internal_declarations: 
    return_type_declaration parameter_declarations variable_declarations has_one_or_more_commands has_one_or_more_newlines 
                                { function_internal_declarations_node *fidn = fid_create_node($1, $2, $3, $4); $$ = fidn; }
    ;

return_type_declaration:
    RETURN_TYPE COLON type has_one_or_more_newlines
                                { $$ = $3; }
    ;

parameter_declarations:
    PARAMETER COLON type_declaration has_one_or_more_newlines parameter_declarations
                                { list_node *ln = list_create_node($3); list_link_nodes(ln, $5); $$ = ln; }
    |                           { $$ = NULL; }
    ;

variable_declarations:
    VARIABLE COLON type_declaration has_one_or_more_newlines variable_declarations
                                { list_node *ln = list_create_node($3); list_link_nodes(ln, $5); $$ = ln; }
    |                           { $$ = NULL; }
    ;

has_many_function_declarations:
    NEWLINE has_many_newlines function_declarations
                                { $$ = $3; }
    |                           { $$ = NULL; }
    ;

type_declaration:
    ID TYPE COLON type          { CREATE_STRING($1); type_declaration_node *tdn = td_create_node($1, $4); $$ = tdn; }
    ;

/*  
    COMMANDS 
    COMMANDS 
    COMMANDS 
                */

has_one_or_more_commands: 
    command_list has_many_commands
                                { list_node *ln = list_create_node($1); list_link_nodes(ln, $2); $$ = ln; }
    ;

has_many_commands: 
    SEMICOLON has_many_newlines has_one_or_more_commands
                                { $$ = $3; }
    |                           { $$ = NULL; }
    ;

command_list:
    IF L_PAREN conditional_expression COMMA command_list_then else_statement R_PAREN
                                { command_if_node *cin = cl_create_if_node($3, $5, $6);
                                    command_list_node *cln = cl_create_node(CL_IF, cin); $$ = cln; }
    | DO_WHILE L_PAREN has_one_or_more_commands COMMA stop_condition R_PAREN
                                { command_do_while_node *cdwn = cl_create_do_while_node($3, $5); $$ = cdwn; 
                                    command_list_node *cln = cl_create_node(CL_DO_WHILE, cdwn); $$ = cln; }
    | WHILE L_PAREN stop_condition COMMA has_one_or_more_commands R_PAREN
                                { command_while_node *cwn = cl_create_while_node($3, $5); ;
                                    command_list_node *cln = cl_create_node(CL_WHILE, cwn); $$ = cln; }
    | FOR L_PAREN for_initialization COMMA stop_condition COMMA value_adjust COMMA has_one_or_more_commands R_PAREN
                                { command_for_node *cfn = cl_create_for_node($3, $5, $7, $9);
                                    command_list_node *cln = cl_create_node(CL_FOR, cfn); $$ = cln; }
    | PRINTF L_PAREN STRING has_many_expressions R_PAREN
                                { CREATE_STRING($3);
                                    command_printf_node *cpfn = cl_create_printf_node($3, $4);
                                    command_list_node *cln = cl_create_node(CL_PRINTF, cpfn);
                                    insert_string_in_global_list(new_str);
                                    $$ = cln; }
    | SCANF L_PAREN STRING COMMA BITWISE_AND L_PAREN ID R_PAREN R_PAREN
                                { CREATE_STRING($3);
                                    command_scanf_node *csfn = cl_create_scanf_node($3, $7);
                                    command_list_node *cln = cl_create_node(CL_SCANF, csfn);
                                    $$ = cln; }
    | EXIT L_PAREN expression R_PAREN
                                { command_exit_node *cen = cl_create_exit_node($3); $$ = cen; 
                                    command_list_node *cln = cl_create_node(CL_EXIT, cen); $$ = cln; }
    | RETURN L_PAREN return_internal_expression R_PAREN
                                { command_return_node *crn = cl_create_return_node($3); $$ = crn; 
                                    command_list_node *cln = cl_create_node(CL_RETURN, crn); $$ = cln; }
    | expression
                                { command_list_node *cln = cl_create_node(CL_EXPRESSION, $1); $$ = cln; }
    ;

command_list_then:
    has_one_or_more_commands    { $$ = $1; }
    ;

else_statement:
    COMMA command_list_else     { $$ = $2; }
    |                           { $$ = NULL; }
    ;

command_list_else:
    has_one_or_more_commands    { $$ = $1; }
    ;

for_initialization:
    expression                  { $$ = $1; }
    |                           { $$ = NULL; }
    ;

return_internal_expression:
    expression                  { $$ = $1; }
    |                           { $$ = NULL; }
    ;

/*  
    CONDITIONS 
    CONDITIONS 
    CONDITIONS 
                */

conditional_expression:
    expression                  { $$ = $1; }
    ;

stop_condition:
    expression                  { $$ = $1; }
    ;

value_adjust:
    expression                  { $$ = $1; }
    |                           { $$ = NULL; }
    ;

/*  
    EXPRESSIONS 
    EXPRESSIONS 
    EXPRESSIONS 
                */

has_one_or_more_expressions:
    expression has_many_expressions 
                                { list_node *ln = list_create_node($1); list_link_nodes(ln, $2); $$ = ln; }
    ;

has_many_expressions:
    COMMA has_one_or_more_expressions
                                { $$ = $2; }
    |                           { $$ = NULL; }
    ;

expression:                     
    binary_operator             { expression_node *en = exp_create_node(EXP_BOP, $1); $$ = en; }
    | unary_operator            { expression_node *en = exp_create_node(EXP_UOP, $1); $$ = en; }
    | primary_expression        { expression_node *en = exp_create_node(EXP_PE, $1); $$ = en; }
    | primary_expression_array  { expression_node *en = exp_create_node(EXP_PE_ARRAY, $1); $$ = en; }
    | ternary_operator          { expression_node *en = exp_create_node(EXP_TOP, $1); $$ = en; }
    | function_call             { expression_node *en = exp_create_node(EXP_FUNC_CALL, $1); $$ = en; }
    ;

primary_expression_array:
    primary_expression array    { pe_set_array($1, $2); $$ = $1; }
    ;

ternary_operator:
    TERNARY_CONDITIONAL L_PAREN expression COMMA expression COMMA expression R_PAREN
                                { ternary_operator_node *topn = top_create_node($3, $5, $7); $$ = topn; }
    ;

function_call:
    ID L_PAREN R_PAREN          { function_call_node *fncn = fncc_create_node($1, NULL); $$ = fncn; }
    | ID L_PAREN has_one_or_more_expressions R_PAREN
                                { function_call_node *fncn = fncc_create_node($1, $3); $$ = fncn; }
    ;

binary_operator: 
    PLUS binary_operator_body
                                { binary_operator_node *bopn = bop_create_node(BOP_PLUS, $2); $$ = bopn; }
    | MINUS binary_operator_body
                                { binary_operator_node *bopn = bop_create_node(BOP_MINUS, $2); $$ = bopn; }
    | MULTIPLY binary_operator_body
                                { binary_operator_node *bopn = bop_create_node(BOP_MULTIPLY, $2); $$ = bopn; }
    | DIV binary_operator_body
                                { binary_operator_node *bopn = bop_create_node(BOP_DIV, $2); $$ = bopn; }
    | REMAINDER binary_operator_body
                                { binary_operator_node *bopn = bop_create_node(BOP_REM, $2); $$ = bopn; }
    | LESS_THAN binary_operator_body
                                { binary_operator_node *bopn = bop_create_node(BOP_LESS_THAN, $2); $$ = bopn; }
    | GREATER_THAN binary_operator_body
                                { binary_operator_node *bopn = bop_create_node(BOP_GREATER_THAN, $2); $$ = bopn; }
    | LESS_EQUAL binary_operator_body
                                { binary_operator_node *bopn = bop_create_node(BOP_LESS_EQUAL, $2); $$ = bopn; }
    | GREATER_EQUAL binary_operator_body
                                { binary_operator_node *bopn = bop_create_node(BOP_GREATER_EQUAL, $2); $$ = bopn; }
    | BITWISE_AND binary_operator_body
                                { binary_operator_node *bopn = bop_create_node(BOP_BITWISE_AND, $2); $$ = bopn; }
    | BITWISE_OR binary_operator_body
                                { binary_operator_node *bopn = bop_create_node(BOP_BITWISE_OR, $2); $$ = bopn; }
    | BITWISE_XOR binary_operator_body
                                { binary_operator_node *bopn = bop_create_node(BOP_BITWISE_XOR, $2); $$ = bopn; }
    | LOGICAL_AND binary_operator_body
                                { binary_operator_node *bopn = bop_create_node(BOP_LOGICAL_AND, $2); $$ = bopn; }
    | LOGICAL_OR binary_operator_body
                                { binary_operator_node *bopn = bop_create_node(BOP_LOGICAL_OR, $2); $$ = bopn; }
    | EQUAL binary_operator_body
                                { binary_operator_node *bopn = bop_create_node(BOP_EQUAL, $2); $$ = bopn; }
    | NOT_EQUAL binary_operator_body
                                { binary_operator_node *bopn = bop_create_node(BOP_NOT_EQUAL, $2); $$ = bopn; }
    | ASSIGN binary_operator_body
                                { binary_operator_node *bopn = bop_create_node(BOP_ASSIGN, $2); $$ = bopn; }
    | PLUS_ASSIGN binary_operator_body
                                { binary_operator_node *bopn = bop_create_node(BOP_PLUS_ASSIGN, $2); $$ = bopn; }
    | MINUS_ASSIGN binary_operator_body
                                { binary_operator_node *bopn = bop_create_node(BOP_MINUS_ASSIGN, $2); $$ = bopn; }
    | R_SHIFT binary_operator_body
                                { binary_operator_node *bopn = bop_create_node(BOP_R_SHIFT, $2); $$ = bopn; }
    | L_SHIFT binary_operator_body
                                { binary_operator_node *bopn = bop_create_node(BOP_L_SHIFT, $2); $$ = bopn; }
    ;

binary_operator_body:
    L_PAREN expression COMMA expression R_PAREN 
                                { binary_operator_body_node *bopbn = bopb_create_node($2, $4); $$ = bopbn; }
    ;

unary_operator:
    INC unary_operator_body     { unary_operator_node *uopn = uop_create_node(UOP_PREINC, 0, $2); $$ = uopn; }
    | DEC unary_operator_body   { unary_operator_node *uopn = uop_create_node(UOP_PREDEC, 0, $2); $$ = uopn; }
    | unary_operator_body INC   { unary_operator_node *uopn = uop_create_node(UOP_POSTINC, 1, $1); $$ = uopn; }
    | unary_operator_body DEC   { unary_operator_node *uopn = uop_create_node(UOP_POSTDEC, 1, $1); $$ = uopn; }
    | PLUS unary_operator_body  { unary_operator_node *uopn = uop_create_node(UOP_PLUS, 0, $2); $$ = uopn; }
    | MINUS unary_operator_body { unary_operator_node *uopn = uop_create_node(UOP_MINUS, 0, $2); $$ = uopn; }
    | LOGICAL_NOT unary_operator_body
                                { unary_operator_node *uopn = uop_create_node(UOP_LOGICAL_NOT, 0, $2); $$ = uopn; }
    | BITWISE_NOT unary_operator_body
                                { unary_operator_node *uopn = uop_create_node(UOP_BITWISE_NOT, 0, $2); $$ = uopn; }
    // a macacada a seguir eh pra nao dar conflito
    | MULTIPLY unary_operator_body                          
                                { unary_operator_node *uopn = uop_create_node(UOP_POINTER, 1, $2); $$ = uopn; }
    | MULTIPLY has_one_or_more_pointers unary_operator_body
                                { unary_operator_node *uopn = uop_create_node(UOP_POINTER, 1 + $2, $3); $$ = uopn; }
    ;

unary_operator_body:
    L_PAREN expression R_PAREN  { $$ = $2; }
    ;

primary_expression:
    ID                          { CREATE_STRING($1); primary_expression_node *pe = pe_create_node(PE_ID, new_str); $$ = pe; }
    | NUM_INTEGER               { CREATE_INT($1); primary_expression_node *pe = pe_create_node(PE_INT, new_int); $$ = pe; }
    | CHARACTER                 { CREATE_CHAR($1); primary_expression_node *pe = pe_create_node(PE_CHAR, new_char); $$ = pe; }
    | STRING                    { CREATE_STRING($1); primary_expression_node *pe = pe_create_node(PE_STRING, new_str);
                                       insert_string_in_global_list(new_str); $$ = pe; }

    ;

/*  
    HELPERS
    HELPERS
    HELPERS
            */

has_one_or_more_newlines:
    NEWLINE has_many_newlines   { $$ = 1 + $2; }
    ;

has_many_newlines:
    NEWLINE has_many_newlines   { $$ = 1 + $2; }
    |                           { $$ = 0; }
    ;

type:
    basic_type type_extension   { type_node *tn = tn_create_node($1, $2); $$ = tn; }
    ;

basic_type:
    INT                         { $$ = BT_INT; }
    | CHAR                      { $$ = BT_CHAR; }
    | VOID                      { $$ = BT_VOID; }
    ;

type_extension:
    has_one_or_more_pointers    { CREATE_INT($1); type_extension_node *ten = ten_create_node(TE_POINTERS, new_int); $$ = ten; }  
    | array                     { type_extension_node *ten = ten_create_node(TE_ARRAYS, $1); $$ = ten; }
    |                           { $$ = NULL; }
    ;

has_one_or_more_pointers:
    MULTIPLY has_many_pointers  { $$ = 1 + $2; }
    ;

has_many_pointers:
    MULTIPLY has_many_pointers  { $$ = 1 + $2; }
    |                           { $$ = 0; }
    ;

array:
    L_SQUARE_BRACKET expression R_SQUARE_BRACKET has_many_arrays 
                                { list_node *ln = list_create_node($2); list_link_nodes(ln, $4); $$ = ln; }
    ;

has_many_arrays:
    array                       { $$ = $1; }
    |                           { $$ = NULL;}
    ;

%%
