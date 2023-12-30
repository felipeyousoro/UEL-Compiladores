#ifndef CONTEXT_MANAGER_H
#define CONTEXT_MANAGER_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "data_structures/list/list.h"
#include "data_structures/stack/stack.h"
#include "data_structures/symbol_table/symbol_table.h"

#define GET_CURRENT_CONTEXT() (symbol_table *) stk_peek(CONTEXT_SYMBOLS)
#define GET_CURRENT_ARGUMENTS_MAP() (char **) stk_peek(CONTEXT_ARGUMENTS_MAP)
#define GET_CURRENT_VARIABLES_MAP() (char **) stk_peek(CONTEXT_VARIABLES_MAP)
#define GET_NEW_LABEL() LABEL_COUNTER; LABEL_COUNTER++;
#define GET_NEXT_STRING_LABEL() STRING_LABEL_COUNTER; STRING_LABEL_COUNTER++;
#define NO_OF_TEMPORARY_REGISTERS 10
#define NO_OF_SAVE_REGISTERS 8
#define NO_OF_ARGUMENTS 4

int LABEL_COUNTER;
int STRING_LABEL_COUNTER;
stack *CONTEXT_SYMBOLS;
stack *CONTEXT_ARGUMENTS_MAP;
stack *CONTEXT_VARIABLES_MAP;
int TEMPORARY_REGISTERS_COUNT;

enum {
    SW_GLOBAL,
    POST_INC,
    POST_DEC,
};

void cm_init_context_manager();

void cm_new_scope();
void cm_exit_scope();

int cm_save_context();
void cm_restore_context(int registers_in_use);

char **cm_create_variables_map();
void cm_variables_save_map(char **map, char *reg_id);
int cm_variables_get_register(char **map, char *reg_id);

char **cm_create_arguments_map();
void cm_arguments_save_map(char **map, char *reg_id);
int cm_arguments_get_register(char **map, char *reg_id);

int cm_get_available_temp_register();
// If register passed is not a temporary register, it will not be freed
void cm_free_register(int reg);

int is_register_temporary(int reg);
#endif