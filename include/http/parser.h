#ifndef HTTP_PARSER_H
#define HTTP_PARSER_H
#include <stdio.h>
#include <string.h>
#include "structs.h"
char *lex_next(char **input, ParserState *current_state);
void parse_all(char *input);
#endif