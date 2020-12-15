#ifndef COURSEWORK_DIFFERENTIATOR_H
#define COURSEWORK_DIFFERENTIATOR_H

#include "tree.h"

// create a constant node.
tree_node* constant_node(double value)
{
    token token;
    token_init_constant(&token, value);
    return tree_node_create(token);
}

// build a tree with an addition root node.
tree_node* add_nodes(tree_node* lhs, tree_node* rhs)
{
    token token;
    token_init(&token, addition, '\0', 0);

    tree_node* node = tree_node_create(token);
    node->left_child = lhs;
    node->right_child = rhs;

    return node;
}

// build a tree with a subtraction root node.
tree_node* subtract_nodes(tree_node* lhs, tree_node* rhs)
{
    token token;
    token_init(&token, subtraction, '\0', 0);

    tree_node* node = tree_node_create(token);
    node->left_child = lhs;
    node->right_child = rhs;

    return node;
}

// build a tree with a multiplication root node.
tree_node* multiply_nodes(tree_node* lhs, tree_node* rhs)
{
    token token;
    token_init(&token, multiplication, '\0', 0);

    tree_node* node = tree_node_create(token);
    node->left_child = lhs;
    node->right_child = rhs;

    return node;
}

// build a tree with a division root node.
tree_node* divide_nodes(tree_node* lhs, tree_node* rhs)
{
    token token;
    token_init(&token, division, '\0', 0);

    tree_node* node = tree_node_create(token);
    node->left_child = lhs;
    node->right_child = rhs;

    return node;
}

// build a tree with a power root node.
tree_node* power_nodes(tree_node* lhs, tree_node* rhs)
{
    token token;
    token_init(&token, power, '\0', 0);

    tree_node* node = tree_node_create(token);
    node->left_child = lhs;
    node->right_child = rhs;

    return node;
}

// build a tree with a unary negation root node.
tree_node* negate_node(tree_node* operand)
{
    token token;
    token_init(&token, negation, '\0', 0);

    tree_node* node = tree_node_create(token);
    node->right_child = operand;

    return node;
}

// build a tree with a squareroot root node.
tree_node* sqrt_node(tree_node* operand)
{
    token token;
    token_init(&token, squareroot, '\0', 0);

    tree_node* node = tree_node_create(token);
    node->left_child = operand;

    return node;
}

// build a tree with a natural logarithm root node.
tree_node* ln_node(tree_node* operand)
{
    token token;
    token_init(&token, log_e, '\0', 0);

    tree_node* node = tree_node_create(token);
    node->left_child = operand;

    return node;
}

// build a tree with a sine root node.
tree_node* sin_node(tree_node* operand)
{
    token token;
    token_init(&token, sine, '\0', 0);

    tree_node* node = tree_node_create(token);
    node->left_child = operand;

    return node;
}

// build a tree with a cosine root node.
tree_node* cos_node(tree_node* operand)
{
    token token;
    token_init(&token, cosine, '\0', 0);

    tree_node* node = tree_node_create(token);
    node->left_child = operand;

    return node;
}

// build a tree with a tangent root node.
tree_node* tan_node(tree_node* operand)
{
    token token;
    token_init(&token, tangent, '\0', 0);

    tree_node* node = tree_node_create(token);
    node->left_child = operand;

    return node;
}

// build an expression tree that represents the derivative of another tree.
tree_node* differentiate(tree_node* node, char symbol)
{
    token current_token = node->token;

    switch(current_token.type) // differentiate with respect to symbol.
    {
        case addition:
            // d/dx[f(x) + g(x)] = f'(x) + g'(x)
            return add_nodes(differentiate(node->left_child, symbol), differentiate(node->right_child, symbol));
        case subtraction:
            // d/dx[f(x) - g(x)] = f'(x) - g'(x)
            return subtract_nodes(differentiate(node->left_child, symbol), differentiate(node->right_child, symbol));
        case multiplication:
            // d/dx[f(x) * g(x)] = f(x) * g'(x) + f'(x) * g(x)
            return add_nodes(multiply_nodes(tree_node_copy(node->left_child), differentiate(node->right_child, symbol)),
                             multiply_nodes(differentiate(node->left_child, symbol), tree_node_copy(node->right_child)));
        case division:
            // d/dx[f(x) / g(x)] = (g(x) * f'(x) - f(x) * g'(x)) / (g(x) ^ 2)
            return divide_nodes(subtract_nodes(multiply_nodes(tree_node_copy(node->right_child), differentiate(node->left_child, symbol)),
                                               multiply_nodes(tree_node_copy(node->left_child), differentiate(node->right_child, symbol))),
                                power_nodes(tree_node_copy(node->right_child), constant_node(2)));
        case negation:
            // d/dx[-f(x)] = -f'(x)
            return negate_node(differentiate(node->right_child, symbol));
        case power:
            // d/dx[f(x) ^ g(x)] = f(x) ^ (g(x) - 1) * (g(x) * f'(x) + f(x) * ln(f(x)) * g'(x))
            return multiply_nodes(power_nodes(tree_node_copy(node->left_child), subtract_nodes(tree_node_copy(node->right_child), constant_node(1))),
                                  add_nodes(multiply_nodes(tree_node_copy(node->right_child), differentiate(node->left_child, symbol)),
                                            multiply_nodes(multiply_nodes(tree_node_copy(node->left_child), differentiate(node->right_child, symbol)),
                                                           ln_node(tree_node_copy(node->left_child)))));
        case squareroot:
            // d/dx[sqrt(f(x))] = f'(x) / (2 * sqrt(f(x)))
            return divide_nodes(differentiate(node->left_child, symbol),
                                multiply_nodes(constant_node(2), sqrt_node(tree_node_copy(node->left_child))));
        case log_10:
            // d/dx[log_10(f(x))] = f'(x) / (ln(10) * f(x))
            return divide_nodes(differentiate(node->left_child, symbol),
                                multiply_nodes(ln_node(constant_node(10)),
                                               tree_node_copy(node->left_child)));
        case log_e:
            // d/dx[ln(f(x))] = f'(x) / f(x)
            return divide_nodes(differentiate(node->left_child, symbol), tree_node_copy(node->left_child));
        case sine:
            // d/dx[sin(f(x))] = cos(f(x)) * f'(x)
            return multiply_nodes(cos_node(tree_node_copy(node->left_child)),
                                  differentiate(node->left_child, symbol));
        case cosine:
            // d/dx[cos(f(x))] = -sin(f(x)) * f'(x)
            return multiply_nodes(negate_node(sin_node(tree_node_copy(node->left_child))),
                                  differentiate(node->left_child, symbol));
        case tangent:
            // d/dx[tan(f(x))] = f'(x) / (cos(f(x))) ^ 2
            return divide_nodes(differentiate(node->left_child, symbol),
                                power_nodes(cos_node(tree_node_copy(node->left_child)),
                                            constant_node(2)));
        case constant:
            // d/dx[c] = 0
            return constant_node(0);
        case variable:
            // d/dx[x] = 1 and d/dx[y] = 0
            return constant_node((current_token.symbol == symbol) ? 1 : 0);
        default:
            // not a valid token.
            return NULL;
    }
}

#endif //COURSEWORK_DIFFERENTIATOR_H
