#include "dll.h"

int insert_at_last(Dlist **head, Dlist **tail, int data)
{
    Dlist *new = malloc(sizeof(Dlist));

    if(new == NULL)
    {
        return FAILURE;
    }

    new->data = data;
    new->prev = NULL;
    new->next = NULL;

    if(*head == NULL)
    {
        *head = new;
        *tail = new;
        return SUCCESS;
    }

    new->prev = *tail;
    (*tail)->next = new;
    *tail = new;

    return SUCCESS;
}

int insert_first(Dlist **head, Dlist **tail, int data)
{
    Dlist *new = malloc(sizeof(Dlist));
    if(new == NULL)
    {
        return FAILURE;
    }
    new->data = data;
    new->next = NULL;
    new->prev = NULL;
    if(*head == NULL && *tail == NULL)
    {
        *head = new;
        *tail = new;
        return SUCCESS;
    }
    new->next = *head;
    (*head)->prev = new;
    *head = new;
    return SUCCESS;
    
}

void print_list(Dlist *head)
{
    while(head != NULL)
    {
        printf("%d", head->data);
        head = head->next;
    }

    printf("\n");
}

void delete_list(Dlist **head, Dlist **tail)
{
    Dlist *temp = *head;

    while(temp)
    {
        Dlist *next = temp->next;
        free(temp);
        temp = next;
    }

    *head = NULL;
    *tail = NULL;
}