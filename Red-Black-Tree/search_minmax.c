#include "tree.h"

int search_RBT(Tree_t *root, int data)
{
    Tree_t *current = root;

    if (root == NULL)
        return FAILURE;

    while (current != NULL)
    {
        if (data == current->data)
            return SUCCESS;
        current = (data < current->data) ? current->left : current->right;
    }

    return NOELEMENT;
}

int findmin(Tree_t *root)
{
    Tree_t *current = root;

    if (current == NULL)
        return FAILURE;

    while (current->left != NULL)
        current = current->left;

    return current->data;
}

int findmax(Tree_t *root)
{
    Tree_t *current = root;

    if (current == NULL)
        return FAILURE;

    while (current->right != NULL)
        current = current->right;

    return current->data;
}
