#ifndef COURSEWORK_INFIX_WRITER_H
#define COURSEWORK_INFIX_WRITER_H

#include <stdio.h>
#include <string.h>

#include "tree.h"

// concatenates five strings in ascending order.
char* concat(const char* str1, const char* str2, const char* str3, const char* str4, const char* str5)
{
    // determine the length of each string.
    int len1 = (str1 == NULL) ? 0 : strlen(str1);
    int len2 = (str2 == NULL) ? 0 : strlen(str2);
    int len3 = (str3 == NULL) ? 0 : strlen(str3);
    int len4 = (str4 == NULL) ? 0 : strlen(str4);
    int len5 = (str5 == NULL) ? 0 : strlen(str5);

    // allocate memory for the concatenated string on the heap.
    char* str = (char*)malloc(sizeof(char) * (len1 + len2 + len3 + len4 + len5 + 1));

    // concatenate the strings.
    int n = 0;
    for (int i = 0; i < len1; ++i) { str[n++] = str1[i]; }
    for (int i = 0; i < len2; ++i) { str[n++] = str2[i]; }
    for (int i = 0; i < len3; ++i) { str[n++] = str3[i]; }
    for (int i = 0; i < len4; ++i) { str[n++] = str4[i]; }
    for (int i = 0; i < len5; ++i) { str[n++] = str5[i]; }

    str[len1 + len2 + len3 + len4 + len5] = '\0'; // null terminator.

    return str;
}

// produces a string in infix notation from the expression tree.
char* infix(tree_node* node)
{
    // inorder traverse the tree.
    if (node != NULL)
    {
        token current_token = node->token;
        bool function = is_function(current_token);

        // Set the prefix string if it is a function.
        char* prefix_string = (char*)malloc(sizeof(char) * 6);
        prefix_string[0] = '\0';
        if (function)
        {
            switch (current_token.type)
            {
                case squareroot: strcpy(prefix_string, "sqrt("); break;
                case log_10: strcpy(prefix_string, "log("); break;
                case log_e: strcpy(prefix_string, "ln("); break;
                case sine: strcpy(prefix_string, "sin("); break;
                case cosine: strcpy(prefix_string, "cos("); break;
                case tangent: strcpy(prefix_string, "tan("); break;
                default: break;
            }
        }
        else if (is_operator(current_token) && is_binary(current_token))
        {
            strcpy(prefix_string, "("); // don't put a left parenthesis around the unary negation.
        }

        // traverse the left subtree.
        char* left_string = infix(node->left_child);

        // visit the root node.
        char* middle_string = (char*)malloc(sizeof(char) * 64);
        middle_string[0] = '\0';
        if (function == false)
        {
            switch (current_token.type)
            {
                case addition: strcpy(middle_string, " + "); break;
                case subtraction: strcpy(middle_string, " - "); break;
                case multiplication: strcpy(middle_string, " * "); break;
                case division: strcpy(middle_string, " / "); break;
                case negation: strcpy(middle_string, "-"); break;
                case power: strcpy(middle_string, ")^("); break;
                case variable: middle_string[0] = current_token.symbol; middle_string[1] = '\0'; break;
                case constant: sprintf(middle_string, "%g", current_token.value); break;
                default: break;
            }
        }

        // traverse the right subtree.
        char* right_string = infix(node->right_child);

        // set the suffix string if it is a function.
        char* suffix_string = malloc(sizeof(char) * 2);
        suffix_string[0] = '\0';
        if (function)
        {
            strcpy(suffix_string, ")");
        }
        else if (is_operator(current_token) && is_binary(current_token))
        {
            strcpy(suffix_string, ")"); // don't put a right parenthesis around the unary negation.
        }

        // allocate a string, which is set to contain the concatenation of the five strings.
        char* expression_string = concat(prefix_string, left_string, middle_string, right_string, suffix_string);

        // free the memory for these five strings.
        free(prefix_string);
        free(left_string);
        free(middle_string);
        free(right_string);
        free(suffix_string);

        return expression_string;
    }

    return NULL; // expression tree is null.
}

#endif //COURSEWORK_INFIX_WRITER_H
