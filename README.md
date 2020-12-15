# Expression Calculator

## Introduction
This is an implementation of an expression calculator in the C programming language. 
The program parses mathematical expressions specified in infix notation into reverse polish notation and then an abstract syntax tree (AST).
The program will then interpet and evaluate the expression, compute the first-order partial derivatives, then interpret and evaluate each of those.

## Features
A list of features is provided below:
* Parsing expressions via shunting-yard algorithm: https://en.wikipedia.org/wiki/Shunting-yard_algorithm.
* Evaluating expressions.
* Computing first-order partial derivatives.
* Simplifying multiply by one, multiply by zero and addition by zero expressions.
* Real numbers and constants.
* Binary and unary operators.
* Functions and variables.

## Mathematical Constants
* pi (π = 3.14159...)
* e  (e = 2.71828...)

## Operators
* \+        (addition) 
* \-        (subtraction)
* \*        (multiplication)
* \/        (division)
* \^        (power)
* \- or \~  (unary negation)

## Functions
* sqrt      (square root)
* log       (log base 10)
* ln        (log base e)
* sin       (sine, argument in radians)
* cos       (cosine, argument in radians)
* tan       (tangent, argument in radians)

## Variables
* a single lowercase letter between a and z excluding e.
* a single uppercase letter between A and Z excluding E.

## Examples
Say our expression F, contains the variables x, y and z. The program will prompt the user to set a value for each variable, evaluate F, compute the partial
derivatives ∂F/∂x, ∂F/∂y, ∂F/∂z and then evaluate each of those.

#### Example 1
<pre>
Enter an expression: 2 + 3 * (2 ^ 5 + 1)
F() = (2 + (3 * ((2)^(5) + 1))) = 101
</pre>

#### Example 2
<pre>
Enter an expression: x * y
x = 1
y = 2
F() = (x * y) = 2
∂F/∂x = y = 2
∂F/∂y = x = 1
</pre>

#### Example 3
<pre>
Enter an expression: sin(2 * x)
x = 0
F() = sin((2 * x)) = 0
∂F/∂x = (cos((2 * x)) * 2) = 2
</pre>
