#include "dll.h"

/* Multiplies every digit of List1 by a single digit, handling carry.
   Same digit-by-digit pattern as addition.c */
int multiply_single_digit(Dlist **head1, Dlist **tail1, int digit, Dlist **headR, Dlist **tailR)
{
    Dlist *temp1 = *tail1;
    int carry = 0, data, rem;

    while(temp1)
    {
        data = (temp1->data * digit) + carry;
        rem = data % 10;
        carry = data / 10;

        insert_first(headR, tailR, rem);

        temp1 = temp1->prev;
    }

    if(carry)
    {
        insert_first(headR, tailR, carry);
    }

    return SUCCESS;
}

int multiplication(Dlist **head1, Dlist **tail1, Dlist **head2, Dlist **tail2, Dlist **headR, Dlist **tailR)
{
    Dlist *temp2 = *tail2;

    Dlist *headR1 = NULL, *tailR1 = NULL;   /* running total */
    int zero_count = 0;

    while(temp2)
    {
        Dlist *headR2 = NULL, *tailR2 = NULL;   /* this round's partial product */
        Dlist *headSum = NULL, *tailSum = NULL;
        int i;

        /* Step: Multiply every digit of List1 */
        multiply_single_digit(head1, tail1, temp2->data, &headR2, &tailR2);

        /* Step: Append zeros according to count (the positional shift) */
        for(i = 0; i < zero_count; i++)
        {
            insert_at_last(&headR2, &tailR2, 0);
        }

        if(headR1 == NULL)
        {
            /* first digit's partial product becomes the initial running total */
            headR1 = headR2;
            tailR1 = tailR2;
        }
        else
        {
            /* Step: addition(headR1, headR2) -> store in headR */
            addition(&headR1, &tailR1, &headR2, &tailR2, &headSum, &tailSum);

            /* Step: Delete headR1, Delete headR2 */
            delete_list(&headR1, &tailR1);
            delete_list(&headR2, &tailR2);

            /* Step: headR1 = headR, headR = NULL */
            headR1 = headSum;
            tailR1 = tailSum;
        }

        /* Next digit */
        temp2 = temp2->prev;
        zero_count++;
    }

    if(headR1)
    {
        remove_leading_zeros(&headR1, &tailR1);
    }

    *headR = headR1;
    *tailR = tailR1;

    return SUCCESS;
}