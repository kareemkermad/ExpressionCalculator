#ifndef COURSEWORK_STACK_H
#define COURSEWORK_STACK_H

#include "list.h"

typedef list stack; // stack is implemented as a doubly linked list.

// initialise the stack.
void stack_init(stack* stack)
{
    list_init(stack);
}

// push an item to the top of the stack.
void stack_push(stack* stack, void* payload)
{
    list_push_back(stack, payload);
}

// remove an item from the top of the stack.
void stack_pop(stack* stack)
{
    list_pop_back(stack);
}

// return an item at the top of the stack.
void* stack_peek(stack* stack)
{
    return list_peek_back(stack);
}

// free memory for each item in the stack.
void stack_free_nodes(stack* stack)
{
    list_free_nodes(stack);
}

#endif //COURSEWORK_STACK_H
