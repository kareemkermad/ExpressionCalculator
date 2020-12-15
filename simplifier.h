#ifndef COURSEWORK_SIMPLIFIER_H
#define COURSEWORK_SIMPLIFIER_H

#include <stdbool.h>

#include "tree.h"
#include "token.h"

bool is_constant(tree_node* node, double value)
{
    if (node != NULL)
    {
        return node->token.type == constant && node->token.value == value;
    }

    return false;
}

// traverses the tree and simplifies multiply-by-one expressions.
bool simplify_multiply_by_one_recursive(tree_node* node)
{
    // post-order traverse the tree.
    // this will simplify children before simplifying parents.
    if (node != NULL)
    {
        tree_node* left = node->left_child;
        tree_node* right = node->right_child;

        bool left_simplified = simplify_multiply_by_one_recursive(left); // simplify the left subtree.
        bool right_simplified = simplify_multiply_by_one_recursive(right); // simplify the right subtree.

        bool simplified = left_simplified || right_simplified;

        if (node->token.type == multiplication)
        {
            if (is_constant(left, 1.0))
            {
                // if the left child is one, the new root node is the right subtree.
                node->token = right->token;
                node->left_child = right->left_child;
                node->right_child = right->right_child;

                simplified = true;

                tree_node_free(left); // free the constant one node.
                tree_node_free(right); // free the right child, which is now the new root node.
            }
            else if (is_constant(right, 1.0))
            {
                // if the right child is one, the new root node is the left subtree.
                node->token = left->token;
                node->left_child = left->left_child;
                node->right_child = left->right_child;

                simplified = true;

                tree_node_free(left); // free the left child, which is now the new root node.
                tree_node_free(right); // free the constant one node.
            }
        }

        return simplified;
    }

    return false;
}

// simplifies any multiply-by-one expressions.
bool simplify_multiply_by_one(tree_node* root)
{
    return simplify_multiply_by_one_recursive(root);
}

// traverses the tree and simplifies multiply-by-zero expressions.
bool simplify_multiply_by_zero_recursive(tree_node* node)
{
    // post-order traverse the tree.
    // this will simplify children before simplifying parents.
    if (node != NULL)
    {
        tree_node* left = node->left_child;
        tree_node* right = node->right_child;

        bool left_simplified = simplify_multiply_by_zero_recursive(left); // simplify the left subtree.
        bool right_simplified = simplify_multiply_by_zero_recursive(right); // simplify the right subtree.

        bool simplified = left_simplified || right_simplified;

        if (node->token.type == multiplication)
        {
            if (is_constant(left, 0.0) || is_constant(right, 0.0))
            {
                // if the left or right child is zero, the new root node is a constant zero.
                token_init_constant(&(node->token), 0.0);
                node->left_child = NULL;
                node->right_child = NULL;

                simplified = true;

                // free the left and right subtrees.
                tree_free(left);
                tree_free(right);
            }
        }

        return simplified;
    }

    return false;
}

// simplifies any multiply-by-zero expressions.
bool simplify_multiply_by_zero(tree_node* root)
{
    return simplify_multiply_by_zero_recursive(root);
}

// traverses the tree and simplifies addition-by-zero expressions.
bool simplify_addition_by_zero_recursive(tree_node* node)
{
    // post-order traverse the tree.
    // this will simplify children before simplifying parents.
    if (node != NULL)
    {
        tree_node* left = node->left_child;
        tree_node* right = node->right_child;

        bool left_simplified = simplify_addition_by_zero_recursive(left); // simplify the left subtree.
        bool right_simplified = simplify_addition_by_zero_recursive(right); // simplify the right subtree.

        bool simplified = left_simplified || right_simplified;

        if (node->token.type == addition)
        {
            if (is_constant(left, 0.0))
            {
                // if the left child is zero, the new root node is the right subtree.
                node->token = right->token;
                node->left_child = right->left_child;
                node->right_child = right->right_child;

                simplified = true;

                tree_node_free(left); // free the constant zero node.
                tree_node_free(right); // free the right child, which is now the new root node.
            }
            else if (is_constant(right, 0.0))
            {
                // if the right child is zero, the new root node is the left subtree.
                node->token = left->token;
                node->left_child = left->left_child;
                node->right_child = left->right_child;

                simplified = true;

                tree_node_free(left); // free the left child, which is now the new root node.
                tree_node_free(right); // free the constant zero node.
            }
        }

        return simplified;
    }

    return false;
}

// simplifies any addition-by-zero expressions.
bool simplify_addition_by_zero(tree_node* root)
{
    return simplify_addition_by_zero_recursive(root);
}

// simplifies the expression tree.
void simplify(tree_node* root)
{
    bool simplified = false;

    do // continue to simplify the tree until we can't simplify anymore.
    {
        simplified = false;
        simplified = simplify_multiply_by_one(root) || simplified;
        simplified = simplify_multiply_by_zero(root) || simplified;
        simplified = simplify_addition_by_zero(root) || simplified;
    }
    while (simplified);
}

#endif //COURSEWORK_SIMPLIFIER_H
