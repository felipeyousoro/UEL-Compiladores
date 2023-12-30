#include "register.h"

__register *reg_create_register(int id, int adjacencies_size) {
    __register *r = (__register *) malloc(sizeof(__register));
    r->id = id;
    r->color = -1;
    r->no_neighbors = 0;
    r->adjacencies_size = adjacencies_size;
    r->adjacencies = (__register **) malloc(sizeof(__register *) * adjacencies_size);

    for (int i = 0; i < adjacencies_size; i++) {
        r->adjacencies[i] = NULL;
    }

    return r;
}

void reg_add_adjacency(void *r1, void *r2) {
    __register *reg1 = (__register *) r1;
    __register *reg2 = (__register *) r2;

    if (reg1->adjacencies[reg2->id] != NULL || reg2->adjacencies[reg1->id] != NULL) {
        return;
    }

    reg1->adjacencies[reg2->id] = reg2;
    reg2->adjacencies[reg1->id] = reg1;

    reg1->no_neighbors++;
    reg2->no_neighbors++;
}

void print_adjacencies(__register *r) {
    printf("Register %d: ", r->id);
    for (int i = 0; i < r->adjacencies_size; i++) {
        if (r->adjacencies[i] != NULL) {
            printf("%d ", r->adjacencies[i]->id);
        }
    }
    printf("\n");
}

void free_register(__register *r) {
    free(r->adjacencies);
    free(r);
}

void reg_recalculate_no_neighbors(__register *r) {
    r->no_neighbors = 0;
    for (int i = 0; i < r->adjacencies_size; i++) {
        if (r->adjacencies[i] != NULL) {
            r->no_neighbors++;
        }
    }
}