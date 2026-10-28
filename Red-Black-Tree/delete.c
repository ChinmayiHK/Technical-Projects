#include "tree.h"

static char color_of(const Tree_t *node)
{
    return node == NULL ? 'B' : node->color;
}

static Tree_t *minimum_node(Tree_t *node)
{
    if (node == NULL)
        return NULL;
    while (node->left != NULL)
        node = node->left;
    return node;
}

static Tree_t *maximum_node(Tree_t *node)
{
    if (node == NULL)
        return NULL;
    while (node->right != NULL)
        node = node->right;
    return node;
}

static void transplant(Tree_t **root, Tree_t *old_node, Tree_t *new_node)
{
    if (old_node->parent == NULL)
        *root = new_node;
    else if (old_node == old_node->parent->left)
        old_node->parent->left = new_node;
    else
        old_node->parent->right = new_node;

    if (new_node != NULL)
        new_node->parent = old_node->parent;
}

/* Restore RBT properties after removing a black node.
 * x_parent is required because x can be NULL (the NIL leaf is implicit).
 */
static void fix_delete(Tree_t **root, Tree_t *x, Tree_t *x_parent)
{
    Tree_t *w;

    while (x != *root && color_of(x) == 'B')
    {
        if (x_parent == NULL)
            break;

        if (x == x_parent->left)
        {
            w = x_parent->right;

            if (color_of(w) == 'R')
            {
                w->color = 'B';
                x_parent->color = 'R';
                left_rotate(root, x_parent);
                w = x_parent->right;
            }

            if (w == NULL ||
                (color_of(w->left) == 'B' && color_of(w->right) == 'B'))
            {
                if (w != NULL)
                    w->color = 'R';
                x = x_parent;
                x_parent = x->parent;
            }
            else
            {
                if (color_of(w->right) == 'B')
                {
                    if (w->left != NULL)
                        w->left->color = 'B';
                    w->color = 'R';
                    right_rotate(root, w);
                    w = x_parent->right;
                }

                if (w != NULL)
                {
                    w->color = x_parent->color;
                    if (w->right != NULL)
                        w->right->color = 'B';
                }
                x_parent->color = 'B';
                left_rotate(root, x_parent);
                x = *root;
                x_parent = NULL;
            }
        }
        else
        {
            w = x_parent->left;

            if (color_of(w) == 'R')
            {
                w->color = 'B';
                x_parent->color = 'R';
                right_rotate(root, x_parent);
                w = x_parent->left;
            }

            if (w == NULL ||
                (color_of(w->right) == 'B' && color_of(w->left) == 'B'))
            {
                if (w != NULL)
                    w->color = 'R';
                x = x_parent;
                x_parent = x->parent;
            }
            else
            {
                if (color_of(w->left) == 'B')
                {
                    if (w->right != NULL)
                        w->right->color = 'B';
                    w->color = 'R';
                    left_rotate(root, w);
                    w = x_parent->left;
                }

                if (w != NULL)
                {
                    w->color = x_parent->color;
                    if (w->left != NULL)
                        w->left->color = 'B';
                }
                x_parent->color = 'B';
                right_rotate(root, x_parent);
                x = *root;
                x_parent = NULL;
            }
        }
    }

    if (x != NULL)
        x->color = 'B';
    if (*root != NULL)
    {
        (*root)->color = 'B';
        (*root)->parent = NULL;
    }
}

int delete_RBT(Tree_t **root, int data)
{
    Tree_t *z, *y, *x, *x_parent;
    char original_color;

    if (root == NULL || *root == NULL)
        return FAILURE;

    z = *root;
    while (z != NULL && z->data != data)
        z = (data < z->data) ? z->left : z->right;

    if (z == NULL)
        return NOELEMENT;

    y = z;
    original_color = y->color;

    if (z->left == NULL)
    {
        x = z->right;
        x_parent = z->parent;
        transplant(root, z, z->right);
        if (x != NULL)
            x_parent = x->parent;
    }
    else if (z->right == NULL)
    {
        x = z->left;
        x_parent = z->parent;
        transplant(root, z, z->left);
        if (x != NULL)
            x_parent = x->parent;
    }
    else
    {
        y = minimum_node(z->right);
        original_color = y->color;
        x = y->right;

        if (y->parent == z)
        {
            x_parent = y;
            if (x != NULL)
                x->parent = y;
        }
        else
        {
            x_parent = y->parent;
            transplant(root, y, y->right);
            y->right = z->right;
            y->right->parent = y;
        }

        transplant(root, z, y);
        y->left = z->left;
        y->left->parent = y;
        y->color = z->color;
    }

    free(z);

    if (original_color == 'B')
        fix_delete(root, x, x_parent);
    else if (*root != NULL)
    {
        (*root)->color = 'B';
        (*root)->parent = NULL;
    }

    return SUCCESS;
}

int delete_minimum(Tree_t **root)
{
    Tree_t *node = minimum_node(root == NULL ? NULL : *root);
    if (node == NULL)
        return FAILURE;
    return delete_RBT(root, node->data);
}

int delete_maximum(Tree_t **root)
{
    Tree_t *node = maximum_node(root == NULL ? NULL : *root);
    if (node == NULL)
        return FAILURE;
    return delete_RBT(root, node->data);
}
