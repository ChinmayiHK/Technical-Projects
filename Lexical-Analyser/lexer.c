#include <stdio.h>
#include <string.h>
#include "lexer.h"

char *keywords[] =
{
    "auto",
    "break",
    "case",
    "char",
    "const",
    "continue",
    "default",
    "do",
    "double",
    "else",
    "enum",
    "extern",
    "float",
    "for",
    "goto",
    "if",
    "int",
    "long",
    "register",
    "return",
    "short",
    "signed",
    "sizeof",
    "static",
    "struct",
    "switch",
    "typedef",
    "union",
    "unsigned",
    "void",
    "volatile",
    "while"
};

int check_keyword(char *word)
{
    int i;

    for (i = 0; i < sizeof(keywords) / sizeof(keywords[0]); i++)
    {
        if (strcmp(word, keywords[i]) == 0)
        {
            return 1;
        }
    }

    return 0;
}

void analyze_file(FILE *fp)
{
    int ch;
    char word[50];

    while ((ch = fgetc(fp)) != EOF)
    {
        // Skip spaces, tabs and new lines
        if (ch == ' ' || ch == '\t' || ch == '\n' || ch == '\r')
        {
            continue;
        }

        // IDENTIFIER / KEYWORD
        if ((ch >= 'a' && ch <= 'z') || (ch >= 'A' && ch <= 'Z') || ch == '_')
        {
            int i = 0;

            word[i++] = ch;

            while ((ch = fgetc(fp)) != EOF && ((ch >= 'a' && ch <= 'z') ||
                    (ch >= 'A' && ch <= 'Z') || (ch >= '0' && ch <= '9') || ch == '_'))
            {
                word[i++] = ch;
            }

            if (ch != EOF)
            {
                ungetc(ch, fp);
            }

            word[i] = '\0';

            if (check_keyword(word))
            {
                printf("%s -> KEYWORD\n", word);
            }
            else
            {
                printf("%s -> IDENTIFIER\n", word);
            }

            continue;
        }

        // CONSTANT
        if (ch >= '0' && ch <= '9')
        {
            int i = 0;
            int decimal = 0;

            word[i++] = ch;

            while ((ch = fgetc(fp)) != EOF)
            {
                if (ch >= '0' && ch <= '9')
                {
                    word[i++] = ch;
                }
                else if (ch == '.' && decimal == 0)
                {
                    word[i++] = ch;
                    decimal = 1;
                }
                else
                {
                    ungetc(ch, fp);
                    break;
                }
            }

            word[i] = '\0';

            printf("%s -> CONSTANT\n", word);

            continue;
        }

        // CHARACTER CONSTANT
        if (ch == '\'')
        {
            char constant[10];
            int i = 0;

            constant[i++] = ch;

            while ((ch = fgetc(fp)) != EOF)
            {
                constant[i++] = ch;

                if (ch == '\'')
                {
                    break;
                }
            }

            constant[i] = '\0';

            printf("%s -> CHARACTER CONSTANT\n", constant);

            continue;
        }

        // STRING
        if (ch == '"')
        {
            char string[100];
            int i = 0;

            string[i++] = ch;

            while ((ch = fgetc(fp)) != EOF)
            {
                string[i++] = ch;

                if (ch == '"')
                {
                    break;
                }
            }

            string[i] = '\0';

            printf("%s -> STRING\n", string);

            continue;
        }

        // COMMENTS
        if (ch == '/')
        {
            int next_ch;

            next_ch = fgetc(fp);

            // Single-line comment
            if (next_ch == '/')
            {
                while ((ch = fgetc(fp)) != '\n' && ch != EOF)
                {
                    ;
                }

                continue;
            }

            // Multi-line comment
            else if (next_ch == '*')
            {
                while ((ch = fgetc(fp)) != EOF)
                {
                    if (ch == '*')
                    {
                        next_ch = fgetc(fp);

                        if (next_ch == '/')
                        {
                            break;
                        }
                        else if (next_ch != EOF)
                        {
                            ungetc(next_ch, fp);
                        }
                    }
                }

                continue;
            }

            // Normal division operator
            else
            {
                if (next_ch != EOF)
                {
                    ungetc(next_ch, fp);
                }
            }
        }


        // OPERATOR
        if (ch == '+' || ch == '-' || ch == '*' || ch == '/' || ch == '%' || ch == '=' ||
            ch == '<' || ch == '>' || ch == '!' || ch == '&' || ch == '|')
        {       
            int next_ch;
            next_ch = fgetc(fp);
            if ((ch == '+' && next_ch == '+') || (ch == '-' && next_ch == '-') ||
                (ch == '+' && next_ch == '=') || (ch == '-' && next_ch == '=') ||
                (ch == '*' && next_ch == '=') || (ch == '/' && next_ch == '=') ||
                (ch == '%' && next_ch == '=') || (ch == '=' && next_ch == '=') ||
                (ch == '!' && next_ch == '=') || (ch == '<' && next_ch == '=') ||
                (ch == '>' && next_ch == '=') || (ch == '&' && next_ch == '&') ||
                (ch == '|' && next_ch == '|'))
            {
                printf("%c%c -> OPERATOR\n", ch, next_ch);
            }
            else
            {
                printf("%c -> OPERATOR\n", ch);

                if (next_ch != EOF)
                {
                    ungetc(next_ch, fp);
                }
            }

            continue;
        }

        // SPECIAL CHARACTER

        if (ch == '(')
        {
            printf("%c -> OPENING BRACKET\n", ch);
        }
        else if (ch == ')')
        {
            printf("%c -> CLOSING BRACKET\n", ch);
        }
        else if (ch == '{')
        {
            printf("%c -> OPENING CURLY BRACKET\n", ch);
        }
        else if (ch == '}')
        {
            printf("%c -> CLOSING CURLY BRACKET\n", ch);
        }
        else if (ch == '[')
        {
            printf("%c -> OPENING SQUARE BRACKET\n", ch);
        }
        else if (ch == ']')
        {
            printf("%c -> CLOSING SQUARE BRACKET\n", ch);
        }
        else if (ch == ';')
        {
            printf("%c -> SEMICOLON\n", ch);
        }
        else if (ch == ',')
        {
            printf("%c -> COMMA\n", ch);
        }
        else if (ch == ':')
        {
            printf("%c -> COLON\n", ch);
        }
        else if (ch != ' ' && ch != '\t' && ch != '\n' && ch != '\r')
        {
            printf("%c -> UNKNOWN CHARACTER\n", ch);
        }
    }
}

