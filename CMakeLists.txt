cmake_minimum_required(VERSION 3.15)
project(coursework C)

set(CMAKE_C_STANDARD 99)

add_executable(coursework main.c list.h token.h tree.h tokenizer.h parser.h queue.h stack.h evaluator.h variables.h differentiator.h infix_writer.h simplifier.h)