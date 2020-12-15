#ifndef COURSEWORK_TOKENIZER_H
#define COURSEWORK_TOKENIZER_H

#include <stdbool.h>
#include <math.h>
#include <ctype.h>

#include "token.h"

bool is_whitespace(char c)
{
    return c == ' ' || c == '\t';
}

bool is_end(char c)
{
    return c == '\0';
}

bool is_digit(char c)
{
    return c >= '0' && c <= '9';
}

bool is_decimal_point(char c)
{
    return c == '.';
}

bool is_letter(char c)
{
    return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z');
}

int parse_int(char c)
{
    return c - '0';
}

// skips any whitespace and updates the offset index to where we read from next.
void read_whitespace(const char* expression, int* offset)
{
    while (is_whitespace(expression[*offset])) { ++(*offset); }
}

// case insensitive substring compare.
bool substrcicmp(const char* str, int start, int length, const char* other)
{
    for (int i = 0; i < length; ++i)
    {
        if (tolower(str[start + i]) != tolower(other[i]))
        {
            return false; // strings are not identical.
        }
    }

    return true; // strings are identical.
}

// returns a number from the expression string and updates the offset index to where we read from next.
double read_number(const char* expression, int* offset)
{
    double value, power;
    int index = *offset;
    for (value = 0.0; is_digit(expression[index]); index++)
    {
        value = 10 * value + parse_int(expression[index]);
    }
    if (is_decimal_point(expression[index]))
    {
        index++;
    }
    for (power = 1.0; is_digit(expression[index]); index++)
    {
        value = 10 * value + parse_int(expression[index]);
        power *= 10;
    }
    *offset = index;
    return value / power;
}

// reads a constant from the expression string and updates the offset index to where we read from next.
bool read_constant(const char* expression, int* offset, token* token)
{
    int length = 0;
    while (is_letter(expression[*offset + length]))
    {
        ++length;
    }

    if (length <= 0) { return false; } // not a constant.

    if (substrcicmp(expression, *offset, length, "e") && length == 1) // base of natural logarithm, e.
    {
        token_init_constant(token, M_E);
        (*offset) += length;
        return true;
    }
    else if (substrcicmp(expression, *offset, length, "pi") && length == 2) // constant, π.
    {
        token_init_constant(token, M_PI);
        (*offset) += length;
        return true;
    }

    return false; // not a constant we have defined.
}

// reads a variable from the expression string and updates the offset index to where we read from next.
bool read_variable(const char* expression, int* offset, token* token)
{
    int length = 0;
    while (is_letter(expression[*offset + length]))
    {
        ++length;
    }

    if (length == 1) // a variable name can only be a single letter.
    {
        token_init_variable(token, expression[*offset]);
        ++(*offset);
        return true;
    }

    return false;
}

// reads a function name from the expression string and updates the offset index to where we read from next.
bool read_function(const char* expression, int* offset, token* token)
{
    int length = 0;
    while (is_letter(expression[*offset + length]))
    {
        ++length;
    }

    if (length <= 0) { return false; } // not a function.

    if (substrcicmp(expression, *offset, length, "sqrt")) // square root.
    {
        token_init(token, squareroot, '\0', 0);
        (*offset) += length;
        return true;
    }
    else if (substrcicmp(expression, *offset, length, "log")) // logarithm base 10.
    {
        token_init(token, log_10, '\0', 0);
        (*offset) += length;
        return true;
    }
    else if (substrcicmp(expression, *offset, length, "ln")) // natural logarithm.
    {
        token_init(token, log_e, '\0', 0);
        (*offset) += length;
        return true;
    }
    else if (substrcicmp(expression, *offset, length, "sin")) // sine.
    {
        token_init(token, sine, '\0', 0);
        (*offset) += length;
        return true;
    }
    else if (substrcicmp(expression, *offset, length, "cos")) // cosine.
    {
        token_init(token, cosine, '\0', 0);
        (*offset) += length;
        return true;
    }
    else if (substrcicmp(expression, *offset, length, "tan")) // tangent.
    {
        token_init(token, tangent, '\0', 0);
        (*offset) += length;
        return true;
    }

    return false; // not a function name we have defined.
}

// reads a symbol from the expression string and updates the offset index to where we read from next.
bool read_symbol(const char* expression, int* offset, token* token)
{
    switch (expression[*offset])
    {
        case '(': token_init(token, left_parenthesis, '\0', 0); ++(*offset); return true;
        case ')': token_init(token, right_parenthesis, '\0', 0); ++(*offset); return true;
        case '+': token_init(token, addition, '\0', 0); ++(*offset); return true;
        case '-': token_init(token, subtraction, '\0', 0); ++(*offset); return true;
        case '*': token_init(token, multiplication, '\0', 0); ++(*offset); return true;
        case '/': token_init(token, division, '\0', 0); ++(*offset); return true;
        case '~': token_init(token, negation, '\0', 0); ++(*offset); return true;
        case '^': token_init(token, power, '\0', 0); ++(*offset); return true;
    }

    return false; // symbol is not defined.
}

// returns the token as an out parameter and updates the offset index to where we read from next.
void pop_token(const char* expression, int* offset, token* token)
{
    // skip any whitespace.
    read_whitespace(expression, offset);

    // are we at the end of the expression?
    if (is_end(expression[*offset]))
    {
        token_init(token, end, '\0', 0);
        return;
    }

    // try to read a number.
    if (is_digit(expression[*offset]) || is_decimal_point(expression[*offset]))
    {
        token_init_constant(token, read_number(expression, offset));
        return;
    }

    // try to read a constant.
    if (read_constant(expression, offset, token))
    {
        return;
    }

    // try to read a variable.
    if (read_variable(expression, offset, token))
    {
        return;
    }

    // try to read a function.
    if (read_function(expression, offset, token))
    {
        return;
    }

    // try to read a symbol.
    if (read_symbol(expression, offset, token))
    {
        return;
    }

    // interpret any other input as the end of the expression.
    token_init(token, end, '\0', 0);
}

// returns the token as an out parameter but does not update the offset index.
void peek_token(const char* expression, int offset, token* token)
{
    pop_token(expression, &offset, token);
}

#endif //COURSEWORK_TOKENIZER_H
