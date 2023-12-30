#include "stack.h"

stack* stk_create_stack() { 
    stack *s = (stack*) malloc(sizeof(stack));
    s->top = NULL;
    s->size = 0;
    return s;
}

static stack_node* create_node(void *value) {
    stack_node *node = (stack_node*) malloc(sizeof(stack_node));
    node->value = value;
    node->next = NULL;
    return node;
}

void stk_push(stack *stack, void *value) {
    stack_node *node = create_node(value);
    node->next = stack->top;
    stack->top = node;
    stack->size++;
}

void* stk_pop(stack *stack) { 
    if(stack->size == 0) {
        return NULL;
    }

    stack_node *node = stack->top;
    stack->top = node->next;
    stack->size--;
    void *value = node->value;
    free(node);
    return value;
}

void* stk_peek(stack *stack) {
    if(stack->size == 0) {
        return NULL;
    }

    return stack->top->value;
}