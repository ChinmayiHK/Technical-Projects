#include <stdio.h>
#include <string.h>

#include "types.h"
#include "functions.h"

void save_database(MainNode *head[])
{
    char filename[20];
    int i;
    FILE *fp;
    MainNode *temp;
    SubNode *sub_temp;

    printf("Enter the file name to save the database: ");
    scanf("%19s", filename);

    if (strstr(filename, ".txt") == NULL)
    {
        printf("Error: File name should have .txt extension.\n");
        return;
    }

    fp = fopen(filename, "w");

    if (fp == NULL)
    {
        printf("Unable to open file.\n");
        return;
    }

    for (i = 0; i < 27; i++)
    {
        temp = head[i];

        while (temp != NULL)
        {
            sub_temp = temp->sub_link;

            while (sub_temp != NULL)
            {
                fprintf(fp, "# %d;%s;%d;%s;%d #\n",
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

    fclose(fp);

    printf("Database saved successfully in %s\n", filename);
}