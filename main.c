#include <stdio.h>

#include "parser.h"
#include "evaluator.h"
#include "variables.h"
#include "differentiator.h"
#include "simplifier.h"
#include "infix_writer.h"

int main()
{
    // get the user input.
    char input[256];
    printf("Enter an expression: ");
    fgets(input, 256, stdin);

    tree_node* root = parse_expression(input); // parse input string to an expression tree.

    simplify(root); // simplify the expression tree.

    list variables;
    find_variables(root, &variables); // add each variable in the expression tree to the variables list.
    query_variables(&variables); // ask the user to assign a value for each variable in the list.
    set_variables(root, &variables); // set these values for each variable node in the expression tree.

    double value = evaluate(root); // evaluate the expression tree.
    char* expression_string = infix(root); // produce a string in infix notation for the simplified expression tree.
    printf("F() = %s = %g\n", expression_string, value);
    free(expression_string);

    for (list_node* item = variables.first; item != NULL; item = item->next)
    {
        // calculate the partial derivative of the expression with respect to each variable.
        variable_value* pair = item->payload;
        tree_node* diff_root = differentiate(root, pair->symbol); // construct the derivative expression tree.
        simplify(diff_root); // simplify the derivative expression tree.
        double diff_value = evaluate(diff_root); // evaluate the derivative expression tree.

        char* diff_expression_string = infix(diff_root); // produce a string in infix notation for the derivative.
        printf("∂F/∂%c = %s = %g\n", pair->symbol, diff_expression_string, diff_value);
        free(diff_expression_string);
        tree_free(diff_root);
    }

    free_variables(&variables);
    tree_free(root);
    return 0;
}