#ifndef COURSEWORK_VARIABLES_H
#define COURSEWORK_VARIABLES_H

#include <stdio.h>

#include "tree.h"
#include "list.h"

typedef struct variable_value
{
    char symbol; // name of the variable.
    double value; // value of the variable
} variable_value;

// allocate memory for a variable on the heap.
variable_value* variable_value_alloc()
{
    return (variable_value*)malloc(sizeof(variable_value));
}


// initialise a variable.
void variable_value_init(variable_value* pair, char symbol, double value)
{
    pair->symbol = symbol;
    pair->value = value;
}

// create a variable.
variable_value* variable_value_create(char symbol, double value)
{
    variable_value* pair = variable_value_alloc();
    variable_value_init(pair, symbol, value);
    return pair;
}

// free memory for a variable.
void free_variables(list* variables)
{
    // iterate over the linked list and free each variable.
    for (list_node* item = variables->first; item != NULL; item = item->next)
    {
        variable_value* pair = item->payload;
        free(pair);
    }

    list_free_nodes(variables); // free the list of variables.
}

// traverses the expression tree to find variables and adds them to the variables list.
void find_variables_recursive(tree_node* node, list* variables)
{
    // pre-order traverse the tree.
    if (node != NULL)
    {
        if (node->token.type == variable)
        {
            bool found = false;
            for (list_node* item = variables->first; item != NULL; item = item->next)
            {
                variable_value* pair = item->payload;
                if (pair->symbol == node->token.symbol)
                {
                    found = true; // we have found this variable before because it is already in the list.
                    break;
                }
            }

            if (found == false) // only add a variable to the list if we haven't found it before.
            {
                variable_value* pair = variable_value_create(node->token.symbol, node->token.value);
                list_push_back(variables, pair); // add this variable to the variables list
            }
        }

        find_variables_recursive(node->left_child, variables); // traverse the left subtree.
        find_variables_recursive(node->right_child, variables); // traverse the right subtree.
    }
}

// finds all variables in the expression tree and adds them to the list of variables.
void find_variables(tree_node* node, list* variables)
{
    find_variables_recursive(node, variables);
}

// traverses the expression tree and gives a value to each variable.
void set_variables_recursive(tree_node* node, list* variables)
{
    // pre-order traverse the tree.
    if (node != NULL)
    {
        if (node->token.type == variable)
        {
            for (list_node* item = variables->first; item != NULL; item = item->next)
            {
                variable_value* pair = item->payload;
                if (pair->symbol == node->token.symbol)
                {
                    // set the value of variable node to the same as that in the variables list.
                    node->token.value = pair->value;
                }
            }
        }

        set_variables_recursive(node->left_child, variables); // traverse the left subtree.
        set_variables_recursive(node->right_child, variables); // traverse the right subtree.
    }
}

// sets values for each variable in the expression tree to be the same as that in the variables list.
void set_variables(tree_node* node, list* variables)
{
    set_variables_recursive(node, variables);
}

// asks the user to set a value for each variable in the variables list.
void query_variables(list* variables)
{
    for (list_node* node = variables->first; node != NULL; node = node->next)
    {
        variable_value* pair = node->payload;
        printf("%c = ", pair->symbol);
        scanf("%lf", &(pair->value));
        int p = 0;
    }
}

#endif //COURSEWORK_VARIABLES_H
