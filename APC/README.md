
# Arbitrary Precision Calculator (APC)

## Project Overview

The Arbitrary Precision Calculator (APC) is a command-line calculator designed to perform arithmetic operations on integers that exceed the range of standard integer data types. The project represents each digit of an operand as a node in a doubly linked list and performs arithmetic operations on these lists.

The application supports addition, subtraction, multiplication, and division. It also handles positive and negative operands and provides quotient and remainder for division.

## Objectives

- To perform arithmetic operations on very large integers.
- To overcome the limitations of fixed-size integer data types.
- To represent large numbers using doubly linked lists.
- To implement arithmetic operations using linked-list traversal and digit-by-digit processing.
- To gain practical experience in dynamic memory allocation and modular project development.

## Features

### Addition
- Performs digit-by-digit addition starting from the least significant digit.
- Handles carry propagation.
- Supports operands with different numbers of digits.

### Subtraction
- Performs digit-by-digit subtraction from the least significant digit.
- Handles borrow propagation.
- Removes unnecessary leading zeros from the result.

### Multiplication
- Performs multiplication using digit-by-digit processing.
- Generates partial products.
- Performs positional shifting.
- Adds partial products to obtain the final result.

### Division
- Performs integer division.
- Generates the quotient digit by digit.
- Provides quotient and remainder.
- Handles division-by-zero conditions.

## Technologies and Concepts Used

- C
- Doubly Linked Lists
- Dynamic Memory Allocation
- Pointers
- Structures
- File Organization
- Modular Programming
- Command-Line Arguments
- Arithmetic Algorithms

## Project Structure

```text
APC/
├── main.c
├── main.h
├── dll.c
├── dll.h
├── addition.c
├── subtraction.c
├── multiplication.c
├── division.c
└── compare.c
