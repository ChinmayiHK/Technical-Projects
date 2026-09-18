#include "dll.h"

/* True if the list represents the value 0 (expects a normalized list,
   i.e. remove_leading_zeros already applied) */
int is_zero(Dlist *head)
{
    if(head == NULL)
        return 1;

    return (head->next == NULL && head->data == 0);
}

int division(Dlist **head1, Dlist **tail1,
             Dlist **head2, Dlist **tail2,
             Dlist **headQ, Dlist **tailQ,
             Dlist **headRem, Dlist **tailRem)
{
    Dlist *temp1 = *head1;   /* walk dividend most-significant digit first */

    Dlist *rem_head = NULL, *rem_tail = NULL;   /* running remainder */
    Dlist *q_head = NULL, *q_tail = NULL;       /* quotient, built left to right */

    if(is_zero(*head2))
    {
        printf("Error: Division by zero\n");
        return FAILURE;
    }

    while(temp1)
    {
        /* Step: bring down next digit of dividend into remainder */
        insert_at_last(&rem_head, &rem_tail, temp1->data);
        remove_leading_zeros(&rem_head, &rem_tail);

        int digit = 0;

        /* Step: subtract divisor out of remainder as many times as it fits */
        while(list_compare(rem_head, *head2) == 1)
        {
            Dlist *new_rem_head = NULL, *new_rem_tail = NULL;

            subtraction(&rem_head, &rem_tail, head2, tail2, &new_rem_head, &new_rem_tail);

            delete_list(&rem_head, &rem_tail);

            rem_head = new_rem_head;
            rem_tail = new_rem_tail;

            digit++;
        }

        /* Step: record this digit's quotient value */
        insert_at_last(&q_head, &q_tail, digit);

        temp1 = temp1->next;
    }

    remove_leading_zeros(&q_head, &q_tail);

    *headQ = q_head;
    *tailQ = q_tail;

    *headRem = rem_head;
    *tailRem = rem_tail;

    return SUCCESS;
}