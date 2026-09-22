#include <stdio.h>
#include "lexer.h"

int main(int argc, char *argv[])
{
    FILE *fp;

    if (argc != 2)
    {
        printf("Usage: ./a.out <source_file.c>\n");
        return 1;
    }

    fp = fopen(argv[1], "r");

    if (fp == NULL)
    {
        printf("Error: Unable to open file\n");
        return 1;
    }

    analyze_file(fp);

    fclose(fp);

    return 0;
}