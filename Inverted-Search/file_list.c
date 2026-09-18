#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "types.h"
#include "functions.h"

FileNode *create_file_node(char *filename)
{
    FileNode *new;

    new = malloc(sizeof(FileNode));

    if (new == NULL)
    {
        return NULL;
    }

    strcpy(new->file_name, filename);

    new->link = NULL;

    return new;
}


void insert_file_at_last(char *filename, FileNode **file_head)
{
    FileNode *new;
    FileNode *temp;

    new = create_file_node(filename);

    if (new == NULL)
    {
        return;
    }

    if (*file_head == NULL)
    {
        *file_head = new;

        return;
    }

    temp = *file_head;

    while (temp->link != NULL)
    {
        temp = temp->link;
    }

    temp->link = new;
}


void delete_file_from_list(char *filename, FileNode **file_head)
{
    FileNode *temp;
    FileNode *prev;

    temp = *file_head;
    prev = NULL;

    while (temp != NULL)
    {
        if (strcmp(temp->file_name, filename) == 0)
        {
            if (prev == NULL)
            {
                *file_head = temp->link;
            }
            else
            {
                prev->link = temp->link;
            }

            free(temp);

            return;
        }

        prev = temp;
        temp = temp->link;
    }
}