#include "tree.h"

static char color_of(Tree_t *node)
{
    return node == NULL ? 'B' : node->color;
}

void fix_insert(Tree_t **root, Tree_t *node)
{
    Tree_t *parent;
    Tree_t *grandparent;
    Tree_t *uncle;

    while (node != *root &&
           color_of(node->parent) == 'R')
    {
        parent = node->parent;
        grandparent = parent->parent;

        if (parent == grandparent->left)
        {
            uncle = grandparent->right;

            /* Case 1: parent and uncle are red; recolor. */
            if (color_of(uncle) == 'R')
            {
                parent->color = 'B';
                uncle->color = 'B';
                grandparent->color = 'R';
                node = grandparent;
            }
            else
            {
                /* Case 2: left-right; turn it into left-left. */
                if (node == parent->right)
                {
                    node = parent;
                    left_rotate(root, node);
                    parent = node->parent;
                    grandparent = parent->parent;
                }

                /* Case 3: left-left. */
                parent->color = 'B';
                grandparent->color = 'R';
                right_rotate(root, grandparent);
            }
        }
        else
        {
            uncle = grandparent->left;

            /* Mirror Case 1. */
            if (color_of(uncle) == 'R')
            {
                parent->color = 'B';
                uncle->color = 'B';
                grandparent->color = 'R';
                node = grandparent;
            }
            else
            {
                /* Right-left; turn it into right-right. */
                if (node == parent->left)
                {
                    node = parent;
                    right_rotate(root, node);
                    parent = node->parent;
                    grandparent = parent->parent;
                }

                /* Right-right. */
                parent->color = 'B';
                grandparent->color = 'R';
                left_rotate(root, grandparent);
            }
        }
    }

    if (*root != NULL)
    {
        (*root)->color = 'B';
        (*root)->parent = NULL;
    }
}
