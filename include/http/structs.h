#ifndef HTTP_STRUCTS_H
#define HTTP_STRUCTS_H
typedef enum
{
    START,
    METHOD,
    URL,
    PROTOCOL_VERSION,
    HEADER_NAME,
    HEADER_VALUE,
    BODY,
    BODY_END
} ParserState;
char *look_ahead(char *input);
char *lex_next(char **input, ParserState *current_state);
void parse_all(char *input);
#endif