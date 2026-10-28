#include "tree.h"

void left_rotate(Tree_t **root, Tree_t *x)
{
    Tree_t *y;

    if (x == NULL || x->right == NULL)
        return;

    y = x->right;
    x->right = y->left;

    if (y->left != NULL)
        y->left->parent = x;

    y->parent = x->parent;

    if (x->parent == NULL)
        *root = y;
    else if (x == x->parent->left)
        x->parent->left = y;
    else
        x->parent->right = y;

    y->left = x;
    x->parent = y;
}

void right_rotate(Tree_t **root, Tree_t *x)
{
    Tree_t *y;

    if (x == NULL || x->left == NULL)
        return;

    y = x->left;
    x->left = y->right;

    if (y->right != NULL)
        y->right->parent = x;

    y->parent = x->parent;

    if (x->parent == NULL)
        *root = y;
    else if (x == x->parent->left)
        x->parent->left = y;
    else
        x->parent->right = y;

    y->right = x;
    x->parent = y;
}
