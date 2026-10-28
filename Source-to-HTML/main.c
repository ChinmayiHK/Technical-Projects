#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "source2html.h"

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        printf("Usage: ./source2html <filename.c>\n");
        return 1;
    }

    FILE *src = fopen(argv[1], "r");
    if (src == NULL)
    {
        perror("Error opening input file");
        return 1;
    }

    char output[1024];

    if (strlen(argv[1]) + strlen(".html") >= sizeof(output))
    {
        fprintf(stderr, "Error: Filename is too long.\n");
        fclose(src);
        return 1;
    }

    strcpy(output, argv[1]);
    strcat(output, ".html");

    FILE *dest = fopen(output, "w");
    if (dest == NULL)
    {
        perror("Error creating output file");
        fclose(src);
        return 1;
    }

    fprintf(dest, "<!DOCTYPE html>\n");
    fprintf(dest, "<html>\n<head>\n");
    fprintf(dest, "<meta charset=\"UTF-8\">\n");
    fprintf(dest, "<title>Source to HTML</title>\n");

    fprintf(dest, "<style>\n");
    fprintf(dest, "body { background: black; color: white; margin: 0; }\n");
    fprintf(dest, "pre { font-family: monospace; white-space: pre-wrap; padding: 10px; }\n");
    fprintf(dest, ".keyword { color: yellow; font-weight: bold; }\n");
    fprintf(dest, ".comment { color: cyan; }\n");
    fprintf(dest, ".string { color: magenta; }\n");
    fprintf(dest, ".number { color: violet; }\n");
    fprintf(dest, ".preprocessor { color: pink; }\n");
    fprintf(dest, ".line-number { color: yellow; display: inline-block; width: 35px; user-select: none; }\n");
    fprintf(dest, "</style>\n");

    fprintf(dest, "</head>\n<body>\n<pre>\n");

    source2html(src, dest);

    fprintf(dest, "</pre>\n</body>\n</html>\n");

    fclose(src);
    fclose(dest);

    printf("HTML file generated successfully: %s\n", output);

    return 0;
}
