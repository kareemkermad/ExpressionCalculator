#ifndef COURSEWORK_LIST_H
#define COURSEWORK_LIST_H

#include <stdlib.h>

#include "token.h"

// node of a doubly linked list.
typedef struct list_node
{
    void* payload;
    struct list_node* next;
    struct list_node* prev;
} list_node;

// allocate memory for a list node on the heap.
list_node* list_node_alloc()
{
    return (list_node*)malloc(sizeof(list_node));
}

// free memory for a list node.
void list_node_free(list_node* node)
{
    if (node != NULL)
    {
        free(node);
    }
}

// initialise a list node.
void list_node_init(list_node* node, void* payload)
{
    node->payload = payload;
    node->next = NULL;
    node->prev = NULL;
}

// create a list node.
list_node* list_node_create(void* payload)
{
    list_node* node = list_node_alloc();
    list_node_init(node, payload);
    return node;
}

// doubly linked list.
typedef struct list
{
    list_node* first;
    list_node* last;
} list;

// allocate memory for the list on the heap.
list* list_alloc()
{
    return (list*)malloc(sizeof(list));
}

// free memory for each node in the list.
void list_free_nodes(list* list)
{
    list_node* node = list->first;
    while (node != NULL)
    {
        list_node* next = node->next;
        free(node);
        node = next;
    }
}

// free memory for the list.
void list_free(list* list)
{
    if (list != NULL)
    {
        list_free_nodes(list);
        free(list);
    }
}

// initialise the list.
void list_init(list* list)
{
    list->first = NULL;
    list->last = NULL;
}

// create the list.
list* list_create()
{
    list* list = list_alloc();
    list_init(list);
    return list;
}

// push an item to the back of the list.
void list_push_back(list* list, void* payload)
{
    list_node* node = list_node_create(payload);

    if (list->last != NULL)
    {
        list->last->next = node;
        node->prev = list->last;
    }
    else
    {
        list->first = node;
    }

    list->last = node;
}

// return an item from the back of the list without removing it.
void* list_peek_back(list* list)
{
    list_node* last = list->last;
    if (last != NULL)
    {
        return last->payload;
    }

    return NULL; // the list is empty.
}

// return an item from the front of the list without removing it.
void* list_peek_front(list* list)
{
    list_node* first = list->first;
    if (first != NULL)
    {
        return first->payload;
    }

    return NULL; // the list is empty.
}

// remove an item from the back of the list.
void list_pop_back(list* list)
{
    list_node* last = list->last;
    if (last == NULL)
    {
        return; // don't pop from an empty list.
    }

    list->last = last->prev; // set the new last node to be the penultimate node.
    if (list->last != NULL)
    {
        // if the new last node is not null then set its next pointer to null.
        list->last->next = NULL;
    }
    else
    {
        // the list is now empty, so set the list's first node to null.
        list->first = NULL;
    }

    list_node_free(last); // free the node we removed.
}

// remove an item from the front of the list.
void list_pop_front(list* list)
{
    list_node* first = list->first;
    if (first == NULL)
    {
        return; // don't pop from an empty list.
    }

    list->first = first->next; // set the new first node to be the second node.
    if (list->first != NULL)
    {
        // if the new first node is not null then set its previous pointer to null.
        list->first->prev = NULL;
    }
    else
    {
        // the list is now empty, so set the list's last node to null.
        list->last = NULL;
    }

    list_node_free(first); // free the node we removed.
}

// determine the length of the list.
int list_length(list* list)
{
    int length = 0;
    for (list_node* node = list->first; node != NULL; node = node->next)
    {
        ++length;
    }

    return length;
}

#endif //COURSEWORK_LIST_H
