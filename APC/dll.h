#ifndef DLL_H
#define DLL_H

#define SUCCESS 0
#define FAILURE -1

#include <stdio.h>
#include <stdlib.h>

typedef struct node
{
    int data;
    struct node *prev;
    struct node *next;
} Dlist;

int insert_at_last(Dlist **head, Dlist **tail, int data);
int insert_first(Dlist **head, Dlist **tail, int data);
int addition(Dlist **head1, Dlist **tail1, Dlist **head2, Dlist **tail2, Dlist **headR, Dlist **tailR);
int subtraction(Dlist **head1,Dlist **tail1,Dlist **head2,Dlist **tail2,Dlist **headR,Dlist **tailR);
int list_compare(Dlist *head1, Dlist *head2);
void remove_leading_zeros(Dlist **head, Dlist **tail);
int multiply_single_digit(Dlist **head1, Dlist **tail1, int digit, Dlist **headR, Dlist **tailR);
int multiplication(Dlist **head1, Dlist **tail1, Dlist **head2, Dlist **tail2, Dlist **headR, Dlist **tailR);
void delete_list(Dlist **head, Dlist **tail);
int division(Dlist **head1, Dlist **tail1, Dlist **head2, Dlist **tail2, Dlist **headQ, Dlist **tailQ, Dlist **headRem, Dlist **tailRem);
int is_zero(Dlist *head);
void print_list(Dlist *head);

#endif