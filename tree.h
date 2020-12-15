#ifndef COURSEWORK_TREE_H
#define COURSEWORK_TREE_H

#include <stdlib.h>

#include "token.h"

// node of a binary tree.
typedef struct tree_node
{
    token token;
    struct tree_node* left_child;
    struct tree_node* right_child;
} tree_node;

// allocate memory for a tree node on the heap.
tree_node* tree_node_alloc()
{
    return (tree_node*)malloc(sizeof(tree_node));
}

// free memory for a tree node.
void tree_node_free(tree_node* node)
{
    if (node != NULL)
    {
        free(node);
    }
}

// free memory for each node in the tree.
void tree_free(tree_node* node)
{
    // post-order tree traversal is required to delete the tree.
    if (node != NULL)
    {
        tree_free(node->left_child);
        tree_free(node->right_child);
        free(node);
    }
}

// initialise a tree node.
void tree_node_init(tree_node* node, token token)
{
    node->token = token;
    node->left_child = NULL;
    node->right_child = NULL;
}

// create a tree node.
tree_node* tree_node_create(token token)
{
    tree_node* node = tree_node_alloc();
    tree_node_init(node, token);
    return node;
}

// create a copy of a tree node.
tree_node* tree_node_copy(tree_node* node)
{
    // pre-order tree traversal is required to create a copy of the tree.
    if (node != NULL)
    {
        tree_node* copy = tree_node_create(node->token);
        copy->left_child = tree_node_copy(node->left_child);
        copy->right_child = tree_node_copy(node->right_child);
        return copy;
    }

    return NULL;
}

#endif //COURSEWORK_TREE_H
