#include <stdio.h>
#include <string.h>

int read_and_validate(int argc, char *argv[])
{
    int i;
    int j;
    FILE *fp;
    char *extension;

    if (argc < 2)
    {
        printf("Error: No input files provided\n");

        return 0;
    }

    for (i = 1; i < argc; i++)
    {
        extension = strrchr(argv[i], '.');

        if (extension == NULL)
        {
            printf("Error: %s has no extension\n", argv[i]);

            return 0;
        }

        if (strcmp(extension, ".txt") != 0)
        {
            printf("Error: %s is not a .txt file\n", argv[i]);

            return 0;
        }

        fp = fopen(argv[i], "r");

        if (fp == NULL)
        {
            printf("Error: %s does not exist\n", argv[i]);

            return 0;
        }

        if (fgetc(fp) == EOF)
        {
            printf("Error: %s is empty\n", argv[i]);

            fclose(fp);

            return 0;
        }

        fclose(fp);

        for (j = 1; j < i; j++)
        {
            if (strcmp(argv[i], argv[j]) == 0)
            {
                printf("Error: %s is repeated\n", argv[i]);

                return 0;
            }
        }
    }

    return 1;
}