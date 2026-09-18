#include <stdio.h>
#include <string.h>

#include "types.h"
#include "functions.h"

void search_database(MainNode *head[])
{
    char word[20];
    int index;
    MainNode *temp;
    SubNode *sub_temp;

    printf("Enter the word to search: ");
    scanf("%19s", word);

    index = word[0] - 'a';

    temp = head[index];

    while (temp != NULL)
    {
        if (strcmp(word, temp->word) == 0)
        {
            printf("Word \"%s\" is present in the database.\n", word);

            printf("File count: %d\n", temp->file_count);

            sub_temp = temp->sub_link;

            while (sub_temp != NULL)
            {
                printf("%s : %d\n",
                       sub_temp->file_name,
                       sub_temp->word_count);

                sub_temp = sub_temp->link;
            }

            return;
        }

        temp = temp->main_link;
    }

    printf("Word \"%s\" is not present in the database.\n", word);
}