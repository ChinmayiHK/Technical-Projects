#include <stdio.h>

#include "types.h"
#include "functions.h"

void display_database(MainNode *head[])
{
    int i;
    MainNode *temp;
    SubNode *sub_temp;

    printf("\n");
    printf("Index\tWord\tFile Count\tFile Name\tWord Count\n");

    for (i = 0; i < 27; i++)
    {
        temp = head[i];

        if (temp == NULL)
        {
            continue;
        }

        while (temp != NULL)
        {
            sub_temp = temp->sub_link;

            while (sub_temp != NULL)
            {
                printf("%d\t%s\t%d\t\t%s\t\t%d\n",
                       i,
                       temp->word,
                       temp->file_count,
                       sub_temp->file_name,
                       sub_temp->word_count);

                sub_temp = sub_temp->link;
            }

            temp = temp->main_link;
        }
    }
}