#include "dll.h"

int addition(Dlist **head1,Dlist **tail1,Dlist **head2,Dlist **tail2,Dlist **headR,Dlist **tailR)
{
    Dlist *temp1, *temp2;
    temp1 = *tail1;
    temp2 = *tail2;
    int carry = 0,data,rem = 0;
    while(temp1 || temp2)
    {
            if(temp1 && temp2)
            {
                data = temp1->data + temp2->data + carry;
                if(data > 9)
                {
                    rem = data % 10;
                    insert_first(headR, tailR, rem);
                    carry = 1;
                }
                else
                {
                    insert_first(headR, tailR, data);
                    carry = 0;
                }
                temp1 = temp1->prev;
                temp2 = temp2->prev;
            }
            else if(temp1 != NULL)
            {
                data = temp1->data + carry;
                if(data > 9)
                {
                    rem = data % 10;
                    insert_first(headR, tailR, rem);
                    carry = 1;
                }
                else
                {
                    insert_first(headR, tailR, data);
                    carry = 0;
                }
                temp1 = temp1->prev;
            }
            else if(temp2 != NULL)
            {
                data = temp2->data + carry;
                if(data > 9)
                {
                    rem = data % 10;
                    insert_first(headR, tailR, rem);
                    carry = 1;
                }
                else
                {
                    insert_first(headR, tailR, data);
                    carry = 0;
                }
                temp2 = temp2->prev;
            }
            
    }
    if(carry)
    {
        insert_first(headR, tailR, carry);
    }
    return SUCCESS;
}
