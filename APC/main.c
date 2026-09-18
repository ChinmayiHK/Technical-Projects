#include "main.h"

int read_operand(char *str, Dlist **head, Dlist **tail, int *sign)
{
    int i = 0;
    int data;

    *sign = 1;

    if(str[0] == '+' || str[0] == '-')
    {
        if(str[0] == '-')
        {
            *sign = -1;
        }

        i = 1;

        if(str[i] == '\0')
        {
            return FAILURE;
        }
    }

    while(str[i] != '\0')
    {
        if(str[i] < '0' || str[i] > '9')
        {
            return FAILURE;
        }

        data = str[i] - '0';

        if(insert_at_last(head, tail, data) == FAILURE)
        {
            return FAILURE;
        }

        i++;
    }

    return SUCCESS;
}

int main(int argc, char *argv[])
{
    Dlist *head1 = NULL;
    Dlist *tail1 = NULL;

    Dlist *head2 = NULL;
    Dlist *tail2 = NULL;

    Dlist *headR = NULL;
    Dlist *tailR = NULL;

    Dlist *headQ = NULL;
    Dlist *tailQ = NULL;

    Dlist *headRem = NULL;
    Dlist *tailRem = NULL;

    int sign1;
    int sign2;
    int ret;
    char operator;

    if(argc != 4)
    {
        printf("Usage : ./APC.out <operand1> <operator> <operand2>\n");
        return FAILURE;
    }

    operator = argv[2][0];

    if(operator != '+' && operator != '-' && operator != 'x' && operator != '/')
    {
        printf("Invalid operator\n");
        return FAILURE;
    }

    ret = read_operand(argv[1], &head1, &tail1, &sign1);

    if(ret == FAILURE)
    {
        printf("Invalid operand\n");
        return FAILURE;
    }

    printf("Operand 1 : ");
    
    if(sign1 == -1)
    {
        printf("-");
    }
    
    print_list(head1);

    ret = read_operand(argv[3], &head2, &tail2, &sign2);

    if(ret == FAILURE)
    {
        printf("Invalid operand\n");
        return FAILURE;
    }

    printf("Operand 2 : ");
    
    if(sign2 == -1)
    {
        printf("-");
    }
    
    print_list(head2);

    switch(operator)
    {
        case '+':

            if(sign1 == sign2)
            {
                addition(&head1, &tail1, &head2, &tail2, &headR, &tailR);

                printf("Addition Result : ");
                if(sign1 == -1)
                {
                    printf("-");
                }

                print_list(headR);
            }
            else
            {
                ret = list_compare(head1, head2);

                if(ret == 1)
                {
                    subtraction(&head1, &tail1, &head2, &tail2, &headR, &tailR);

                    if(sign1 == -1)
                    {
                        printf("-");
                    }

                    printf("Addition Result : ");
                    print_list(headR);
                }
                else
                {
                    subtraction(&head2, &tail2, &head1, &tail1, &headR, &tailR);

                    if(sign2 == -1)
                    {
                        printf("-");
                    }
                    
                    printf("Addition Result : ");
                    print_list(headR);
                }
            }

            break;

        case '-':

            if(sign1 != sign2)
            {
                addition(&head1, &tail1, &head2, &tail2, &headR, &tailR);

                if(sign1 == -1)
                {
                    printf("-");
                }

                printf("Subtraction Result : ");
                print_list(headR);
            }
            else
            {
                ret = list_compare(head1, head2);

                if(ret == 1)
                {
                    subtraction(&head1, &tail1, &head2, &tail2, &headR, &tailR);

                    if(sign1 == -1)
                    {
                        printf("-");
                    }

                    printf("Subtraction Result : ");
                    print_list(headR);
                }
                else
                {
                    subtraction(&head2, &tail2, &head1, &tail1, &headR, &tailR);

                    if(sign1 == 1)
                    {
                        printf("-");
                    }

                    printf("Subtraction Result : ");
                    print_list(headR);
                }
            }

            break;

        case 'x':

            multiplication(&head1, &tail1, &head2, &tail2, &headR, &tailR);

            if(sign1 != sign2)
            {
                if(!(headR->data == 0 && headR->next == NULL))
                {
                    printf("-");
                }
            }

            printf("Multiplication Result : ");
            print_list(headR);

            break;

        case '/':

            ret = division(&head1, &tail1, &head2, &tail2, &headQ, &tailQ, &headRem, &tailRem);

            if(ret == FAILURE)
            {
                return FAILURE;
            }

            if(sign1 != sign2)
            {
                if(!(headQ->data == 0 && headQ->next == NULL))
                {
                    printf("-");
                }
            }

            printf("Quotient  : ");
            print_list(headQ);

            if(sign1 == -1)
            {
                if(!(headRem->data == 0 && headRem->next == NULL))
                {
                    printf("-");
                }
            }

            printf("Remainder : ");
            print_list(headRem);

            break;
    }

    return SUCCESS;
}