#ifndef COURSEWORK_PARSER_H
#define COURSEWORK_PARSER_H

#include <string.h>

#include "tree.h"
#include "queue.h"
#include "stack.h"
#include "tokenizer.h"

bool is_operator_symbol(char c)
{
    return c == '+' || c == '-' || c == '*' || c == '/' || c == '~' || c == '^';
}

// rewrites the expression string to differentiate between unary and binary minus symbols.
void preprocess_expression(char* expression)
{
    int length = strlen(expression);
    int previous_index = -1; // index of the previous non whitespace character in the expression string.
    for (int current_index = 0; current_index < length; ++current_index)
    {
        char current = expression[current_index];
        if (is_whitespace(current) == false) // skip whitespace.
        {
            if (current == '-')
            {
                if (previous_index < 0 || expression[previous_index] == '(' || is_operator_symbol(expression[previous_index]))
                {
                    // if the minus symbol is preceded by nothing, an open parenthesis or an operator then
                    // replace the binary minus symbol with a unary minus symbol.
                    expression[current_index] = '~';
                }
            }
            previous_index = current_index; // update the previous index.
        }
    }
}

// parses the expression string using the shunting-yard algorithm and produces an expression tree.
// https://en.wikipedia.org/wiki/Shunting-yard_algorithm
tree_node* parse_expression(const char* expression)
{
    int length = strlen(expression) + 1;
    char* expression_copy = malloc(sizeof(char) * length);
    strcpy(expression_copy, expression);

    preprocess_expression(expression_copy); // differentiate between unary and binary minus symbols.

    // create the output queue.
    queue queue;
    queue_init(&queue);

    // create the operator stack.
    stack stack;
    stack_init(&stack);

    int offset = 0; // the current index into the expression string where we do lexical analysis.

    while (true)
    {
        token* current_token = token_alloc();
        peek_token(expression_copy, offset, current_token); // read the current token.

        if (current_token->type == end)
        {
            break; // there are no more tokens to read.
        }
        else if (is_operand(*current_token))
        {
            // if the token is a number then push it to the output queue.
            queue_enqueue(&queue, current_token);
        }
        else if (is_function(*current_token))
        {
            // if the token is a function then push it onto the operator stack.
            stack_push(&stack, current_token);
        }
        else if (is_operator(*current_token))
        {
            token* top = stack_peek(&stack);
            while (top != NULL && current_token->type != left_parenthesis &&
                (is_function(*top) || precedence(top->type) > precedence(current_token->type) ||
                    (precedence(top->type) == precedence(current_token->type) && is_left_associative(top->type))))
            {
                // pop the operator from the operator stack and enqueue it on the output queue.
                stack_pop(&stack);
                queue_enqueue(&queue, top);
                top = stack_peek(&stack);
            }

            // if the token is an operator then push it onto the operator stack.
            stack_push(&stack, current_token);
        }
        else if (current_token->type == left_parenthesis)
        {
            // if the token is a left parenthesis then push it onto the operator stack.
            stack_push(&stack, current_token);
        }
        else if (current_token->type == right_parenthesis)
        {
            token* top = stack_peek(&stack);
            while (top->type != left_parenthesis)
            {
                // pop the operator from the operator stack and enqueue it on the output queue.
                stack_pop(&stack);
                queue_enqueue(&queue, top);
                top = stack_peek(&stack);
            }
            if (top->type == left_parenthesis)
            {
                // pop the operator from the operator stack and discard it.
                stack_pop(&stack);
                free(top);
            }
        }

        pop_token(expression_copy, &offset, current_token); // move to the next token.
    }

    // pop any remaining operators from the operator stack and enqueue them on the output queue.
    token* top = stack_peek(&stack);
    while (top != NULL)
    {
        stack_pop(&stack);
        queue_enqueue(&queue, top);
        top = stack_peek(&stack);
    }

    // the output queue is in reverse polish notation, so we can build an expression tree using a stack.
    // the stack will now store tree nodes instead of tokens.
    token* current_token = queue_dequeue(&queue);
    while (current_token != NULL)
    {
        if (is_operand(*current_token))
        {
            // if the token is an operand then push it onto the stack.
            tree_node* node = tree_node_create(*current_token);
            stack_push(&stack, node);
        }
        else if (is_operator(*current_token))
        {
            tree_node* node = tree_node_create(*current_token); // create a new tree node for this operator.

            // if the token is an operator then we need to pop at least one node from the stack.
            // for a unary operator, we make this node the right child of the operator node.
            node->right_child = stack_peek(&stack);
            stack_pop(&stack);

            if (is_binary(*current_token))
            {
                // if the token is a binary operator then pop another node from the stack.
                // make this node the the left child of the operator node.
                node->left_child = stack_peek(&stack);
                stack_pop(&stack);
            }

            stack_push(&stack, node); // push this new node onto the stack.
        }
        else if (is_function(*current_token))
        {
            // if the token is a function then pop one node from the stack and make it its left child.
            tree_node* node = tree_node_create(*current_token);
            node->left_child = stack_peek(&stack);
            stack_pop(&stack);
            stack_push(&stack, node);
        }

        free(current_token); // free the current token.
        current_token = queue_dequeue(&queue); // read the next token in the output queue.
    }

    tree_node* root = stack_peek(&stack); // the root of the expression tree will be the last node in the stack.
    stack_pop(&stack);

    // the stack and queue must be empty at this point, so we don't need to free their nodes.
    free(expression_copy);
    return root;
}

#endif //COURSEWORK_PARSER_H
