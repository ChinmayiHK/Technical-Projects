#include "dll.h"

int list_compare(Dlist *head1, Dlist *head2)
{
    int len1 = 0, len2 = 0;

    Dlist *temp1 = head1;
    Dlist *temp2 = head2;

    while(temp1)
    {
        len1++;
        temp1 = temp1->next;
    }

    while(temp2)
    {
        len2++;
        temp2 = temp2->next;
    }

    if(len1 > len2)
        return 1;

    if(len1 < len2)
        return 0;

    temp1 = head1;
    temp2 = head2;

    while(temp1 && temp2)
    {
        if(temp1->data > temp2->data)
            return 1;

        if(temp1->data < temp2->data)
            return 0;

        temp1 = temp1->next;
        temp2 = temp2->next;
    }

    return 1;
}

void remove_leading_zeros(Dlist **head, Dlist **tail)
{
    while(*head != NULL && (*head)->data == 0 && *head != *tail)
    {
        Dlist *temp = *head;

        *head = (*head)->next;

        (*head)->prev = NULL;

        free(temp);
    }
}