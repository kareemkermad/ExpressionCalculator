#ifndef COURSEWORK_QUEUE_H
#define COURSEWORK_QUEUE_H

#include "list.h"

typedef list queue; // queue is implemented as a doubly linked list.

// initialise the queue.
void queue_init(queue* queue)
{
    list_init(queue);
}

// add an item to the end of the queue.
void queue_enqueue(queue* queue, void* payload)
{
    list_push_back(queue, payload);
}

// remove and return an item from the front of the queue.
void* queue_dequeue(queue* queue)
{
    void* payload = list_peek_front(queue);
    list_pop_front(queue);
    return payload;
}

// free memory for each item in the queue.
void queue_free_nodes(queue* queue)
{
    list_free_nodes(queue);
}

#endif //COURSEWORK_QUEUE_H
