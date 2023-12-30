#include "context_manager.h"
#include "macros.h"

int LABEL_COUNTER = 0;
int STRING_LABEL_COUNTER = 0;
stack *CONTEXT_SYMBOLS = NULL;
stack *CONTEXT_ARGUMENTS_MAP = NULL;
stack *CONTEXT_VARIABLES_MAP = NULL;

int TEMPORARY_REGISTERS_COUNT = 0;

void cm_init_context_manager() {
    LABEL_COUNTER = 0;
    CONTEXT_SYMBOLS = stk_create_stack();
    CONTEXT_ARGUMENTS_MAP = stk_create_stack();
    CONTEXT_VARIABLES_MAP = stk_create_stack();

    symbol_table *global_context = st_create_table();
    stk_push(CONTEXT_SYMBOLS, global_context);
}

void cm_new_scope() {
    TEMPORARY_REGISTERS_COUNT = 0;
    symbol_table *new_context = st_create_table();
    symbol_table *current_context = GET_CURRENT_CONTEXT();

    char **args_map = cm_create_arguments_map();
    stk_push(CONTEXT_ARGUMENTS_MAP, args_map);

    char **vars_map = cm_create_variables_map();
    stk_push(CONTEXT_VARIABLES_MAP, vars_map);

    for (list_node *current = current_context->symbols; current != NULL; current = current->next) {
        st_push(new_context, current->value);
    }

    stk_push(CONTEXT_SYMBOLS, new_context);
}

void cm_exit_scope() {
    stk_pop(CONTEXT_SYMBOLS);
    stk_pop(CONTEXT_ARGUMENTS_MAP);
}

char **cm_create_variables_map() {
    char **map = malloc(NO_OF_TEMPORARY_REGISTERS * sizeof(char *));
    for (int i = 0; i < NO_OF_TEMPORARY_REGISTERS; i++) {
        map[i] = 0;
    }

    return map;
}

void cm_variables_save_map(char **map, char *reg_id) {
    //printf("\t\t\tSALVENDO REGISTRO %s\n", reg_id);
    for (int i = 0; i < NO_OF_TEMPORARY_REGISTERS; i++) {
        if (map[i] == 0) {
            map[i] = reg_id;
            return;
        }
    }
}

int cm_variables_get_register(char **map, char *reg_id) {
    for (int i = 0; i < NO_OF_TEMPORARY_REGISTERS; i++) {
        if (map[i] != 0 && strcmp(map[i], reg_id) == 0) {
            return i + 16;
        }
    }

    return -1;
}

char **cm_create_arguments_map() {
    char **map = malloc(NO_OF_ARGUMENTS * sizeof(char *));
    for (int i = 0; i < NO_OF_ARGUMENTS; i++) {
        map[i] = 0;
    }

    return map;
}

void cm_arguments_save_map(char **map, char *reg_id) {
    for (int i = 0; i < NO_OF_ARGUMENTS; i++) {
        if (map[i] == 0) {
            map[i] = reg_id;
            return;
        }
    }
}

int cm_arguments_get_register(char **map, char *reg_id) {
    for (int i = 0; i < NO_OF_ARGUMENTS; i++) {
        if (map[i] != 0 && strcmp(map[i], reg_id) == 0) {
            return i + 4;
        }

    }

    return -1;
}

static int get_temp_register_number(int reg) {
    if (reg < 8) { // t0-t7
        return reg + 8;
    }
    else {          // t8-t9
        return reg + 16;
    }
}
int cm_get_available_temp_register() {
    if(TEMPORARY_REGISTERS_COUNT < 0) {
        TEMPORARY_REGISTERS_COUNT = 0;
    }

    if (TEMPORARY_REGISTERS_COUNT < NO_OF_TEMPORARY_REGISTERS) {
        return get_temp_register_number(TEMPORARY_REGISTERS_COUNT++);
    }
    else {
        TEMPORARY_REGISTERS_COUNT++;
        int return_value = get_temp_register_number(TEMPORARY_REGISTERS_COUNT % NO_OF_TEMPORARY_REGISTERS);
        SAVE_STACK(return_value);
        return return_value;
    }
}

void cm_free_register(int reg) {
    if ( !((reg >= 8 && reg <= 15) || reg == 24 || reg == 25) ) {
        return;
    }

    if (TEMPORARY_REGISTERS_COUNT <= NO_OF_TEMPORARY_REGISTERS) {
        TEMPORARY_REGISTERS_COUNT--;
    }
    else {
        int return_value = get_temp_register_number(TEMPORARY_REGISTERS_COUNT % NO_OF_TEMPORARY_REGISTERS);
        LOAD_STACK(return_value);
        TEMPORARY_REGISTERS_COUNT--;
    }

}

int cm_save_context() {
    int total_to_save = TEMPORARY_REGISTERS_COUNT % NO_OF_TEMPORARY_REGISTERS;

    MIPS_ADDI(29, 29, -4 * total_to_save);
    for (int i = 0; i < total_to_save; i++) {
        MIPS_SW(get_temp_register_number(i), 4 * i, 29);
    }

    MIPS_ADDI(29, 29, -4 * NO_OF_ARGUMENTS);
    for (int i = 0; i < NO_OF_ARGUMENTS; i++) {
        MIPS_SW(7 - i, 4 * i, 29);
    }

    MIPS_ADDI(29, 29, -4 * NO_OF_SAVE_REGISTERS);
    for (int i = 0; i < NO_OF_SAVE_REGISTERS; i++) {
        MIPS_SW(23 - i, 4 * i, 29);
    }

    SAVE_STACK(31);

    return total_to_save;
}

void cm_restore_context(int total_to_save) {
    LOAD_STACK(31);

    for (int i = NO_OF_SAVE_REGISTERS - 1; i >= 0; i--) {
        MIPS_LW(23 - i, 4 * i, 29);
    }
    MIPS_ADDI(29, 29, 4 * NO_OF_SAVE_REGISTERS);

    for (int i = NO_OF_ARGUMENTS - 1; i >= 0; i--) {
        MIPS_LW(7 - i, 4 * i, 29);
    }
    MIPS_ADDI(29, 29, 4 * NO_OF_ARGUMENTS);

    for (int i = total_to_save - 1; i >= 0; i--) {
        MIPS_LW(get_temp_register_number(i), 4 * i, 29);
    }
    MIPS_ADDI(29, 29, 4 * total_to_save);

}

int is_register_temporary(int reg) {
    return (reg >= 8 && reg <= 15) || reg == 24 || reg == 25;
}