#ifndef STACK_H
#define STACK_H

#include <stdlib.h>
#include <string.h>

typedef struct stack_node {
    void *value;
    struct stack_node *next;
} stack_node;

typedef struct stack {
    stack_node *top;
    int size;
} stack;

stack* stk_create_stack();

void stk_push(stack *stack, void *value);

void* stk_pop(stack *stack);
void* stk_peek(stack *stack);

#endif


