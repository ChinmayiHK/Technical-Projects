#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "source2html.h"

void print_html_char(int ch, FILE *dest)
{
    if (ch == '<')
        fputs("&lt;", dest);
    else if (ch == '>')
        fputs("&gt;", dest);
    else if (ch == '&')
        fputs("&amp;", dest);
    else
        fputc(ch, dest);
}

int is_keyword(const char *word)
{
    const char *keywords[] = {
        "int", "char", "float", "double", "void",
        "if", "else", "for", "while", "do",
        "return", "break", "continue", "switch",
        "case", "default", "struct", "typedef",
        "const", "static", "unsigned", "long",
        "short", "signed", "sizeof", "enum",
        "extern", "register", "volatile", "auto",
        "union", "goto", "inline", "restrict",
        "_Bool", "_Complex", "_Imaginary",
        NULL
    };

    for (int i = 0; keywords[i] != NULL; i++)
    {
        if (strcmp(word, keywords[i]) == 0)
            return 1;
    }

    return 0;
}

void source2html(FILE *src, FILE *dest)
{
    int ch, next;
    char word[256];
    int i;
    int line_number = 1;
    int line_start = 1;

    while ((ch = fgetc(src)) != EOF)
    {
        if (line_start)
        {
            fprintf(dest,
                    "<span class=\"line-number\">%d</span>",
                    line_number++);
            line_start = 0;
        }

        /* Preprocessor directives */
        if (ch == '#')
        {
            fputs("<span class=\"preprocessor\">", dest);
            print_html_char(ch, dest);

            while ((ch = fgetc(src)) != EOF && ch != '\n')
                print_html_char(ch, dest);

            fputs("</span>", dest);

            if (ch == '\n')
            {
                fputc('\n', dest);
                line_start = 1;
            }

            continue;
        }

        /* Comments */
        if (ch == '/')
        {
            next = fgetc(src);

            if (next == '/')
            {
                fputs("<span class=\"comment\">//", dest);

                while ((ch = fgetc(src)) != EOF && ch != '\n')
                    print_html_char(ch, dest);

                fputs("</span>", dest);

                if (ch == '\n')
                {
                    fputc('\n', dest);
                    line_start = 1;
                }

                continue;
            }
            else if (next == '*')
            {
                fputs("<span class=\"comment\">/*", dest);
                int previous = 0;

                while ((ch = fgetc(src)) != EOF)
                {
                    print_html_char(ch, dest);

                    if (ch == '\n')
                    {
                        line_number++;
                        line_start = 1;
                        fprintf(dest,
                                "<span class=\"line-number\">%d</span>",
                                line_number);
                        line_start = 0;
                    }

                    if (previous == '*' && ch == '/')
                        break;

                    previous = ch;
                }

                fputs("</span>", dest);
                continue;
            }
            else
            {
                fputc('/', dest);

                if (next != EOF)
                    ungetc(next, src);

                continue;
            }
        }

        /* Strings and character constants */
        if (ch == '"' || ch == '\'')
        {
            int quote = ch;

            fputs("<span class=\"string\">", dest);
            print_html_char(ch, dest);

            while ((ch = fgetc(src)) != EOF)
            {
                print_html_char(ch, dest);

                if (ch == '\\')
                {
                    next = fgetc(src);
                    if (next != EOF)
                        print_html_char(next, dest);
                    continue;
                }

                if (ch == quote)
                    break;

                if (ch == '\n')
                {
                    line_number++;
                    line_start = 1;
                }
            }

            fputs("</span>", dest);
            continue;
        }

        /* Keywords and identifiers */
        if (isalpha((unsigned char)ch) || ch == '_')
        {
            i = 0;
            word[i++] = ch;

            while ((ch = fgetc(src)) != EOF &&
                   (isalnum((unsigned char)ch) || ch == '_'))
            {
                if (i < 254)
                    word[i++] = ch;
            }

            word[i] = '\0';

            if (is_keyword(word))
                fputs("<span class=\"keyword\">", dest);

            for (int j = 0; word[j] != '\0'; j++)
                print_html_char(word[j], dest);

            if (is_keyword(word))
                fputs("</span>", dest);

            if (ch != EOF)
                ungetc(ch, src);

            continue;
        }

        /* Numeric constants */
        if (isdigit((unsigned char)ch))
        {
            fputs("<span class=\"number\">", dest);
            print_html_char(ch, dest);

            while ((ch = fgetc(src)) != EOF &&
                   (isalnum((unsigned char)ch) || ch == '.'))
                print_html_char(ch, dest);

            fputs("</span>", dest);

            if (ch != EOF)
                ungetc(ch, src);

            continue;
        }

        print_html_char(ch, dest);

        if (ch == '\n')
            line_start = 1;
    }
}
