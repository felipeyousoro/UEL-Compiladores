%{
    #include <stdio.h>
    #include <stdlib.h>
    #include <string.h>

    #include "graph.h"
    #include "register.h"
    #include "list.h"

    extern int yylex();

    static void yyerror(char *s) {
        fprintf(stderr, "%s\n", s);
    }

    typedef struct reg_interferences {
        int reg_number;
        list_node* interferences;

    } reg_interferences;


    static int max_size = -1;

    #define UPDATE_MAX_SIZE(x) if (x > max_size) max_size = x;

    graph *registers_graph = NULL;
    list_node *registers_to_print = NULL;

    void add_to_registers_to_print(__register *reg) {
        if (registers_to_print == NULL) {
            registers_to_print = list_create_node(reg);
        }
        else {
            list_node *current = registers_to_print;
            while(current->next != NULL) {
                current = current->next;
            }
            list_node *new_node = list_create_node(reg);
            list_link_nodes(current, new_node);
        }

    }

    int k_size = -1;
    int graph_number = -1;

%}

%union{
    int integer;
    void* pointer;
}

%token GRAPH
%token COLON
%token ASSIGN
%token NEWLINE
%token ARROW
%token K
%token <integer> NUMBER
%token END_OF_FILE

%type <pointer> has_one_or_more_reg_interferences
%type <pointer> has_many_reg_interferences
%type <pointer> has_one_or_more_interferences
%type <pointer> has_many_interferences
%type <pointer> register_interferences

%start program

%%

program:
    graph_definition k_size has_one_or_more_reg_interferences END_OF_FILE
                { registers_graph = grf_create_graph(max_size + 1);
                    for(int i = 0; i < registers_graph->size; i++) {
                        grf_add_vertex(registers_graph, i, reg_create_register(i, registers_graph->size));
                    }

                    for(list_node *current = $3; current != NULL; current = current->next) {
                        reg_interferences *r1 = current->value;
                        add_to_registers_to_print(grf_get_vertex(registers_graph, r1->reg_number));
                        for(list_node *current_interference = r1->interferences; current_interference != NULL; current_interference = current_interference->next) {
                            int interference = *(int*)current_interference->value;
                            grf_add_edge(registers_graph, r1->reg_number, interference, reg_add_adjacency);
                            free(current_interference->value);
                        }
                        list_free(r1->interferences);
                        free(r1);
                    }
                    list_free($3);

                    return 0;
                }
    ;

graph_definition:
    GRAPH NUMBER COLON has_many_newlines { graph_number = $2; }
    ;

k_size:
    K ASSIGN NUMBER has_many_newlines { k_size = $3; }
    ;

has_one_or_more_reg_interferences:
    register_interferences has_one_or_more_newlines has_many_reg_interferences
                { list_node* reg_interferences_node = list_create_node($1);
                    list_link_nodes(reg_interferences_node, $3);
                    $$ = reg_interferences_node;
                }
    | register_interferences
                { list_node* reg_interferences_node = list_create_node($1);
                    $$ = reg_interferences_node;
                }
    ;

has_many_reg_interferences:
    has_one_or_more_reg_interferences
                { $$ = $1; }
    |
                { $$ = NULL; }
    ;

register_interferences:
    NUMBER ARROW has_one_or_more_interferences
                { reg_interferences* reg_interferences = malloc(sizeof(reg_interferences));
                    reg_interferences->reg_number = $1;
                    reg_interferences->interferences = $3;
                    $$ = reg_interferences;

                    UPDATE_MAX_SIZE($1);
                }
    ;

has_one_or_more_interferences:
    NUMBER has_many_interferences
                { int *reg_number = malloc(sizeof(int));

                    *reg_number = $1;
                    list_node* reg_number_node = list_create_node(reg_number);
                    list_link_nodes(reg_number_node, $2);

                    $$ = reg_number_node;
                }
    ;

has_many_interferences:
    has_one_or_more_interferences
                { $$ = $1; }
    |
                { $$ = NULL; }
    ;

has_one_or_more_newlines:
    NEWLINE has_many_newlines
    ;

has_many_newlines:
    has_one_or_more_newlines
    |
    ;

%%
