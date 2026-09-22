#ifndef LEXER_H
#define LEXER_H

#include <stdio.h>

typedef enum
{
    KEYWORD,
    IDENTIFIER,
    CONSTANT,
    OPERATOR,
    SPECIAL_CHARACTER
} TokenType;

void analyze_file(FILE *fp);

int check_keyword(char *word);

#endif