#ifndef RBT_H
#define RBT_H

#include <stdio.h>
#include <stdlib.h>

#define SUCCESS 0
#define FAILURE -1
#define NOELEMENT -2
#define DUPLICATE -3

typedef struct node
{
    struct node *left;
    int data;
    struct node *right;
    char color;                 /* 'R' or 'B' */
    struct node *parent;
} Tree_t;

int insert_into_RBT(Tree_t **root, int data);
void left_rotate(Tree_t **root, Tree_t *x);
void right_rotate(Tree_t **root, Tree_t *x);
void fix_insert(Tree_t **root, Tree_t *node);

int findmin(Tree_t *root);
int findmax(Tree_t *root);
int search_RBT(Tree_t *root, int data);
int delete_RBT(Tree_t **root, int data);
int delete_minimum(Tree_t **root);
int delete_maximum(Tree_t **root);

void inorder(Tree_t *root);
void preorder(Tree_t *root);
void postorder(Tree_t *root);
void display_inorder_colored(Tree_t *root);
void free_tree(Tree_t *root);

#endif
