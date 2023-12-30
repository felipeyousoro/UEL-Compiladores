#include <stdio.h>

#include "sintatico.tab.h"
#include "graph.h"
#include "list.h"
#include "register.h"
#include "stack.h"

extern graph *registers_graph;
extern list_node *registers_to_print;
extern int graph_number;
extern int k_size;

int compare_registers_smaller_no_neighbors(const void *r1, const void *r2) {
    __register *reg1 = *(__register **) r1;
    __register *reg2 = *(__register **) r2;

    if (reg1 == NULL && reg2 == NULL) return 0;
    if (reg1 == NULL || reg2 == NULL) return reg1 == NULL ? 1 : -1;

    if (reg1->no_neighbors == reg2->no_neighbors) {
        return reg1->id - reg2->id;
    }

    return reg1->no_neighbors - reg2->no_neighbors;
}

int compare_registers_greater_no_neighbors(const void *r1, const void *r2) {
    __register *reg1 = *(__register **) r1;
    __register *reg2 = *(__register **) r2;

    if (reg1 == NULL && reg2 == NULL) return 0;
    if (reg1 == NULL || reg2 == NULL) return reg1 == NULL ? 1 : -1;

    if (reg1->no_neighbors == reg2->no_neighbors) {
        return reg1->id - reg2->id;
    }

    return reg2->no_neighbors - reg1->no_neighbors;
}

void remove_neighbors(__register *r) {
    for (int i = 0; i < r->adjacencies_size; i++) {
        if (r->adjacencies[i] != NULL) {
            r->adjacencies[i]->no_neighbors--;
        }
    }
}

void print_push_register(__register *__reg, int __allocable_registers) {
    if (!list_contains(registers_to_print, __reg)) {
        return;
    }

    printf("Push: %d", __reg->id);
    if (__reg->no_neighbors > __allocable_registers - 1) {
        printf(" *");
    }
    printf("\n");

}

stack *simplify_registers(__register **__registers, int __virtual_registers,
                          int __allocable_registers) {
    stack *s = stk_create_stack();

    for (int i = 0; i < __virtual_registers; i++) {
        qsort(__registers, __virtual_registers, sizeof(__register *), compare_registers_smaller_no_neighbors);
        __register *reg = __registers[0];

        if (reg->no_neighbors > __allocable_registers - 1) {
            qsort(__registers, __virtual_registers, sizeof(__register *), compare_registers_greater_no_neighbors);
            reg = __registers[0];
        }

        __registers[0] = NULL;

        stk_push(s, reg);
        print_push_register(reg, __allocable_registers);
        remove_neighbors(reg);
    }

    return s;
}

void color_physical_registers(graph *__registers_graph, int __physical_registers) {
    for (int i = 0; i < __physical_registers; i++) {
        __register *reg = (__register *) grf_get_vertex(__registers_graph, i);
        reg->color = i;
    }
}

void color_virtual_registers(graph *__registers_graph, int __physical_registers, int __virtual_registers) {
    for (int i = 0; i < __virtual_registers; i++) {
        __register *reg = (__register *) grf_get_vertex(__registers_graph, i + __physical_registers);
        reg->color = -1;
    }
}

int find_available_color(__register *__reg, int __physical_registers) {
    int colors[__physical_registers];

    for (int i = 0; i < __physical_registers; i++) {
        colors[i] = 0;
    }

    for (int i = 0; i < __reg->adjacencies_size; i++) {
        if (__reg->adjacencies[i] != NULL && __reg->adjacencies[i]->color != -1) {
            colors[__reg->adjacencies[i]->color] = 1;
        }
    }

    for (int i = 0; i < __physical_registers; i++) {
        if (colors[i] == 0) {
            return i;
        }
    }

}

void print_pop_register(__register *__reg, int __allocable_registers) {
    if (!list_contains(registers_to_print, __reg)) {
        return;
    }

    printf("Pop: %d ->", __reg->id);
    if (__reg->color > __allocable_registers - 1) {
        printf(" NO COLOR AVAILABLE");
    } else {
        printf(" %d", __reg->color);
    }
    printf("\n");

}

int select_registers(graph *__registers_graph, stack *simplify, int __physical_registers, int __virtual_registers,
                     int __allocable_registers) {

    color_physical_registers(__registers_graph, __physical_registers);
    color_virtual_registers(__registers_graph, __physical_registers, __virtual_registers);

    while (stk_peek(simplify) != NULL) {
        __register *reg = stk_pop(simplify);
        reg->color = find_available_color(reg, __physical_registers);
        print_pop_register(reg, __allocable_registers);

        if (reg->color > __allocable_registers - 1) {
            return 0;
        }

    }

    return 1;
}

int allocate_registers(graph *__registers_graph, int __physical_registers, int __virtual_registers,
                       int __allocable_registers) {
    printf("----------------------------------------\n");
    printf("K = %d\n\n", __allocable_registers);

    __register *simplify[__virtual_registers];

    int simplify_index = 0;
    for (int i = __physical_registers; i < grf_get_size(__registers_graph); i++) {
        __register *reg = (__register *) grf_get_vertex(__registers_graph, i);
        reg_recalculate_no_neighbors(reg);
        simplify[simplify_index] = reg;

        simplify_index++;
    }

    stack *s = simplify_registers(simplify, __virtual_registers, __allocable_registers);

    int result = select_registers(__registers_graph, s, __physical_registers, __virtual_registers,
                                  __allocable_registers);

    return result;
}


int main(int argc, char **argv) {
    yyparse();
    //yylex_destroy();

    printf("Graph %d -> Physical Registers: %d\n", graph_number, k_size);
    printf("----------------------------------------\n");


    int allocation_status[k_size];
    for (int i = k_size; i > 1; i--) {
        int virtual_registers = grf_get_size(registers_graph) - k_size;
        int result = allocate_registers(registers_graph, k_size, virtual_registers, i);
        allocation_status[i] = result;
    }

    printf("----------------------------------------\n");
    printf("----------------------------------------\n");

    int alignment = 0;
    for (int i = k_size; i > 1; i--) {
        if (i / 10 > alignment) {
            alignment = i / 10;
        }
    }

    for (int i = k_size; i > 1; i--) {
        printf("Graph %d -> K = ", graph_number);
        printf("%*d: ", alignment - (i / 10), i);
        if (allocation_status[i] == 1) {
            printf("Successful Allocation");
        } else {
            printf("SPILL");
        }

        if(i != 2) {
            printf("\n");
        }
    }

    for (int i = 0; i < registers_graph->size; i++) {
        __register *reg = (__register *) grf_get_vertex(registers_graph, i);
        free_register(reg);
    }
    free_graph(registers_graph);

    return 0;
}
