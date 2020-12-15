#ifndef COURSEWORK_TOKEN_H
#define COURSEWORK_TOKEN_H

#include <stdbool.h>

typedef enum token_type
{
    left_parenthesis, // parentheses.
    right_parenthesis,
    addition, // binary operators.
    subtraction,
    multiplication,
    division,
    power,
    negation, // unary operator.
    squareroot, // functions.
    log_10,
    log_e,
    sine,
    cosine,
    tangent,
    constant, // operands.
    variable,
    end // end of input.
} token_type;

typedef struct token
{
    token_type type;
    char symbol; // name of a variable.
    double value; // value of a variable or a constant.
} token;

// allocate memory for a token on the heap.
token* token_alloc()
{
    return (token*)malloc(sizeof(token));
}

// initialise token for a variable.
void token_init_variable(token* token, char symbol)
{
    token->type = variable;
    token->symbol = symbol;
    token->value = 0;
}

// initialise token for a constant.
void token_init_constant(token* token, double value)
{
    token->type = constant;
    token->symbol = '\0';
    token->value = value;
}

// initialise token for anything else.
void token_init(token* token, token_type type, char symbol, double value)
{
    token->type = type;
    token->symbol = symbol;
    token->value = value;
}

bool is_function(token token)
{
    switch(token.type)
    {
        case squareroot:
        case log_10:
        case log_e:
        case sine:
        case cosine:
        case tangent:
            return true;
        default:
            return false;
    }
}

bool is_operand(token token)
{
    return token.type == constant || token.type == variable;
}

bool is_operator(token token)
{
    switch (token.type)
    {
        case addition:
        case subtraction:
        case multiplication:
        case division:
        case power:
        case negation:
            return true;
        default:
            return false;
    }
}

// is token is a unary operator?
bool is_unary(token token)
{
    return token.type == negation;
}

// is token is a binary operator?
bool is_binary(token token)
{
    return is_unary(token) == false; // assuming we have already determined token is an operator.
}

// operators of the same precedence which are evaluated right to left.
bool is_right_associative(token_type type)
{
    return type == negation || type == power;
}

// operators of the same precedence which are evaluated left to right.
bool is_left_associative(token_type type)
{
    return is_right_associative(type) == false; // assuming we have already determined token is an operator.
}

// operators with higher precedence are evaluated first.
int precedence(token_type type)
{
    switch (type)
    {
        case negation:
        case power:
            return 4;
        case multiplication:
        case division:
            return 3;
        case addition:
        case subtraction:
            return 2;
        default:
            return -1; // token is not a valid operator.
    }
}

#endif //COURSEWORK_TOKEN_H
