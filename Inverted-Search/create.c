#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "types.h"
#include "functions.h"


MainNode *create_main_node(char *word)
{
    MainNode *new;

    new = malloc(sizeof(MainNode));

    if (new == NULL)
    {
        return NULL;
    }

    strcpy(new->word, word);

    new->file_count = 0;

    new->sub_link = NULL;

    new->main_link = NULL;

    return new;
}


SubNode *create_sub_node(char *filename)
{
    SubNode *new;

    new = malloc(sizeof(SubNode));

    if (new == NULL)
    {
        return NULL;
    }

    strcpy(new->file_name, filename);

    new->word_count = 1;

    new->link = NULL;

    return new;
}


void store_word(char *word, char *filename, MainNode *head[])
{
    int index;
    MainNode *temp;
    MainNode *new_main;
    SubNode *sub_temp;
    SubNode *new_sub;

    index = tolower(word[0]) - 'a';

    temp = head[index];

    while (temp != NULL)
    {
        if (strcmp(word, temp->word) == 0)
        {
            sub_temp = temp->sub_link;

            while (sub_temp != NULL)
            {
                if (strcmp(filename, sub_temp->file_name) == 0)
                {
                    sub_temp->word_count++;

                    return;
                }

                sub_temp = sub_temp->link;
            }

            new_sub = create_sub_node(filename);

            if (new_sub == NULL)
            {
                return;
            }

            sub_temp = temp->sub_link;

            while (sub_temp->link != NULL)
            {
                sub_temp = sub_temp->link;
            }

            sub_temp->link = new_sub;

            temp->file_count++;

            return;
        }

        temp = temp->main_link;
    }

    new_main = create_main_node(word);

    if (new_main == NULL)
    {
        return;
    }

    new_sub = create_sub_node(filename);

    if (new_sub == NULL)
    {
        free(new_main);

        return;
    }

    new_main->sub_link = new_sub;

    new_main->file_count = 1;

    new_main->main_link = head[index];

    head[index] = new_main;
}

int check_file_present(MainNode *head[], char *filename)
{
    int i;
    MainNode *temp;
    SubNode *sub_temp;

    for (i = 0; i < 26; i++)
    {
        temp = head[i];

        while (temp != NULL)
        {
            sub_temp = temp->sub_link;

            while (sub_temp != NULL)
            {
                if (strcmp(filename, sub_temp->file_name) == 0)
                {
                    return 1;
                }

                sub_temp = sub_temp->link;
            }

            temp = temp->main_link;
        }
    }

    return 0;
}

void create_database(int argc, char *argv[], MainNode *head[])
{
    char word[20];
    int index;
    int ch;
    int i;
    FILE *fp;

    for (i = 1; i < argc; i++)
    {
        if (check_file_present(head, argv[i]) == 1)
        {
            printf("INFO: %s already present in database.\n", argv[i]);

            continue;
        }

        fp = fopen(argv[i], "r");

        if (fp == NULL)
        {
            printf("Unable to open %s\n", argv[i]);

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

                    store_word(word, argv[i], head);

                    index = 0;
                }
            }
        }

        if (index > 0)
        {
            word[index] = '\0';

            store_word(word, argv[i], head);
        }

        fclose(fp);

        printf("INFO: %s added to database.\n", argv[i]);
    }
}