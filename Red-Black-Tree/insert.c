
#include "tree.h"

int insert_into_RBT(Tree_t **root, int data)
{
    Tree_t *parent = NULL;
    Tree_t *current = *root;
    Tree_t *new_node;

    /* Find the correct insertion position */
    while (current != NULL)
    {
        parent = current;

        if (data < current->data)
        {
            current = current->left;
        }
        else if (data > current->data)
        {
            current = current->right;
        }
        else
        {
            return DUPLICATE;
        }
    }

    /* Allocate memory for the new node */
    new_node = malloc(sizeof(Tree_t));

    if (new_node == NULL)
    {
        return FAILURE;
    }

    /* Initialize the new node */
    new_node->data = data;
    new_node->left = NULL;
    new_node->right = NULL;
    new_node->parent = parent;
    new_node->color = 'R';

    /* Attach the node to its parent */
    if (parent == NULL)
    {
        *root = new_node;
    }
    else if (data < parent->data)
    {
        parent->left = new_node;
    }
    else
    {
        parent->right = new_node;
    }

    /* Restore Red-Black Tree properties */
    fix_insert(root, new_node);

    return SUCCESS;
}
