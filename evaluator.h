#ifndef COURSEWORK_EVALUATOR_H
#define COURSEWORK_EVALUATOR_H

#include <math.h>

#include "tree.h"
#include "variables.h"

// evaluates the expression tree recursively.
double evaluate(tree_node* node)
{
    token token = node->token;
    switch (token.type)
    {
        case addition:
            // add the left and right subtrees.
            return evaluate(node->left_child) + evaluate(node->right_child);
        case subtraction:
            // subtract the right subtree from the left subtree.
            return evaluate(node->left_child) - evaluate(node->right_child);
        case multiplication:
            // multiply the left and right subtrees.
            return evaluate(node->left_child) * evaluate(node->right_child);
        case division:
            // divide the left subtree by the right subtree.
            return evaluate(node->left_child) / evaluate(node->right_child);
        case power:
            // left subtree raised to the power of the right subtree.
            return pow(evaluate(node->left_child), evaluate(node->right_child));
        case negation:
            // negate the right subtree.
            return -evaluate(node->right_child);
        case squareroot:
            // square root of the left subtree.
            return sqrt(evaluate(node->left_child));
        case log_10:
            // log base 10 of the left subtree.
            return log10(evaluate(node->left_child));
        case log_e:
            // natural log of the left subtree.
            return log(evaluate(node->left_child));
        case sine:
            // sin of the left subtree.
            return sin(evaluate(node->left_child));
        case cosine:
            // cos of the left subtree.
            return cos(evaluate(node->left_child));
        case tangent:
            // tan of the left subtree.
            return tan(evaluate(node->left_child));
        case constant:
            // value of a constant.
            return token.value;
        case variable:
            // value of a variable.
            return token.value;
        default:
            // not a valid token type.
            return 0;
    }
}

#endif //COURSEWORK_EVALUATOR_H
