#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "types.h"
#include "functions.h"


void update_new_files(FileNode *file_head, MainNode *head[])
{
    FileNode *temp;
    FILE *fp;
    char word[20];
    int index;
    int ch;

    temp = file_head;

    while (temp != NULL)
    {
        fp = fopen(temp->file_name, "r");

        if (fp == NULL)
        {
            printf("Unable to open %s\n", temp->file_name);

            temp = temp->link;

            continue;
        }

        index = 0;

        while ((ch = fgetc(fp)) != EOF)
        {
            if (isalpha(ch))
            {
                if (index < 19)
                {
                    word[index] = tolower(ch);

                    index++;
                }
            }
            else
            {
                if (index > 0)
                {
                    word[index] = '\0';

                    store_word(word, temp->file_name, head);

                    index = 0;
                }
            }
        }

        if (index > 0)
        {
            word[index] = '\0';

            store_word(word, temp->file_name, head);
        }

        fclose(fp);

        printf("INFO: %s updated in database.\n", temp->file_name);

        temp = temp->link;
    }
}


void update_database(int argc, char *argv[], MainNode *head[],
                     FileNode **file_head)
{
    int i;
    FileNode *temp;
    FileNode *next;

    /*
     * Create file list from command-line arguments
     */
    for (i = 1; i < argc; i++)
    {
        insert_file_at_last(argv[i], file_head);
    }

    /*
     * Remove files which are already
     * present in the database
     */
    temp = *file_head;

    while (temp != NULL)
    {
        next = temp->link;

        if (check_file_present(head, temp->file_name) == 1)
        {
            delete_file_from_list(temp->file_name, file_head);
        }

        temp = next;
    }

    /*
     * Check whether any new files are left
     */
    temp = *file_head;

    if (temp == NULL)
    {
        printf("INFO: No new files to update.\n");

        return;
    }

    /*
     * Display files which need to be updated
     */
    printf("Files to be updated:\n");

    while (temp != NULL)
    {
        printf("%s -> ", temp->file_name);

        temp = temp->link;
    }

    printf("NULL\n");

    /*
     * Add the new files to the database
     */
    update_new_files(*file_head, head);
}