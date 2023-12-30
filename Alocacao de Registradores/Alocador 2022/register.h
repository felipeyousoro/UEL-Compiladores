#ifndef COMPILADORES_REGISTER_H
#define COMPILADORES_REGISTER_H

#include <stdio.h>
#include <stdlib.h>

typedef struct __register {
    int id;
    int color;
    int adjacencies_size;
    int no_neighbors;
    struct __register **adjacencies;
} __register;

__register *reg_create_register(int id, int adjacencies_size);

void reg_add_adjacency(void *r, void *r2);

void print_adjacencies(__register *r);

void free_register(__register *r);

void reg_recalculate_no_neighbors(__register *r);

#endif //COMPILADORES_REGISTER_H
