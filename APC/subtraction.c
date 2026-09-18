#include "dll.h"

int subtraction(Dlist **head1, Dlist **tail1, Dlist **head2, Dlist **tail2, Dlist **headR, Dlist **tailR)
{
    Dlist *temp1 = *tail1;
    Dlist *temp2 = *tail2;

    int borrow = 0;
    int diff;

    while(temp1 || temp2)
    {
        int d1 = 0;
        int d2 = 0;

        if(temp1)
            d1 = temp1->data;

        if(temp2)
            d2 = temp2->data;

        d1 = d1 - borrow;

        if(d1 < d2)
        {
            d1 = d1 + 10;
            borrow = 1;
        }
        else
        {
            borrow = 0;
        }

        diff = d1 - d2;

        insert_first(headR, tailR, diff);

        if(temp1)
            temp1 = temp1->prev;

        if(temp2)
            temp2 = temp2->prev;
    }
    remove_leading_zeros(headR, tailR);
    return SUCCESS;
}