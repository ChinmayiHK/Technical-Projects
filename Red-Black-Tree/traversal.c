#include "tree.h"

void inorder(Tree_t *root)
{
    if (root == NULL)
        return;

    inorder(root->left);
    printf("%d ", root->data);
    inorder(root->right);
}

void preorder(Tree_t *root)
{
    if (root == NULL)
        return;

    printf("%d ", root->data);
    preorder(root->left);
    preorder(root->right);
}

void postorder(Tree_t *root)
{
    if (root == NULL)
        return;

    postorder(root->left);
    postorder(root->right);
    printf("%d ", root->data);
}

void display_inorder_colored(Tree_t *root)
{
    if (root == NULL)
        return;

    display_inorder_colored(root->left);
    printf("%d(%c) ", root->data, root->color);
    display_inorder_colored(root->right);
}

void free_tree(Tree_t *root)
{
    if (root == NULL)
        return;

    free_tree(root->left);
    free_tree(root->right);
    free(root);
}
