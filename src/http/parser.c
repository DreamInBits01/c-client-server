#include "http/parser.h"

char *look_ahead(char *input)
{
    if (!input)
        return NULL;
    if (*input == '\0')
        return input;
    return input + 1;
}
char *lex_next(char **input, ParserState *current_state)
{
    char buffer[128] = {0};
    int buffer_index = 0;
    char ahead_character;
    char *current_character_pointer = *input;
    if (*current_state == BODY)
    {
        *current_state = BODY_END;
        return;
    }

    while (*current_character_pointer)
    {
        char current_character = *current_character_pointer;
        switch (*current_state)
        {
        case METHOD:
            while (current_character != ' ' && buffer_index < (sizeof(buffer) - 1))
            {
                buffer[buffer_index] = current_character;
                buffer_index++;
                current_character_pointer++;
                current_character = *current_character_pointer;
            }
            buffer[buffer_index] = '\0';
            printf("Method:%s\n", buffer);
            // Skip space ++
            *input = ++current_character_pointer;
            *current_state = URL;
            return;
        case URL:
            // Second run, accumulate the URL
            while (buffer_index < (sizeof(buffer) - 1))
            {
                if (current_character == ' ')
                {
                    current_character_pointer++; // Skip white space
                    break;
                }
                buffer[buffer_index] = current_character;
                buffer_index++;
                current_character_pointer++;
                current_character = *current_character_pointer;
            }
            buffer[buffer_index] = '\0';
            printf("URL:%s\n", buffer);
            *input = current_character_pointer;
            *current_state = PROTOCOL_VERSION;
            return;
        case PROTOCOL_VERSION:
            while (buffer_index < (sizeof(buffer) - 1))
            {
                current_character = *current_character_pointer;
                if (current_character == '\0')
                    break;
                ahead_character = *look_ahead(current_character_pointer);
                if (current_character == '\r' && ahead_character == '\n')
                {
                    // Skip \r\n
                    if (*current_character_pointer == '\r')
                        current_character_pointer++;
                    if (*current_character_pointer == '\n')
                        current_character_pointer++;
                    break;
                }
                buffer[buffer_index] = current_character;
                buffer_index++;

                current_character_pointer++;
            }
            buffer[buffer_index] = '\0';
            printf("PROTOCOL_VERSION:%s\n", buffer);
            *input = current_character_pointer;
            *current_state = HEADER_NAME;
            return;
        case HEADER_NAME:
            // Need to accumulate headers in a dynamic array
            current_character = *current_character_pointer;
            ahead_character = *look_ahead(current_character_pointer);
            if (current_character == '\r' && ahead_character == '\n')
            {
                // End of headers
                if (*current_character_pointer == '\r')
                    current_character_pointer++;
                if (*current_character_pointer == '\n')
                    current_character_pointer++;
                *input = current_character_pointer;
                *current_state = BODY;
                return;
            }
            while (buffer_index < (sizeof(buffer) - 1))
            {
                current_character = *current_character_pointer;
                if (current_character == ':')
                {
                    current_character_pointer++; // skip :
                    while (*current_character_pointer == ' ')
                        current_character_pointer++;
                    break;
                }
                buffer[buffer_index++] = current_character;
                current_character_pointer++;
            }
            buffer[buffer_index] = '\0';
            printf("HEADER NAME:%s\n", buffer);
            *input = current_character_pointer;
            *current_state = HEADER_VALUE;
            return;
        case HEADER_VALUE:

            while (buffer_index < (sizeof(buffer) - 1))
            {
                current_character = *current_character_pointer;
                char ahead_character = *look_ahead(current_character_pointer);
                if (current_character == '\r' && ahead_character == '\n')
                {
                    if (*current_character_pointer == '\r')
                        current_character_pointer++;
                    if (*current_character_pointer == '\n')
                        current_character_pointer++;
                    break;
                }
                buffer[buffer_index] = current_character;

                buffer_index++;
                current_character_pointer++;
            }
            buffer[buffer_index] = '\0';
            printf("HEADER VALUE:%s\n", buffer);
            *input = current_character_pointer;
            *current_state = HEADER_NAME;
            return;
        case BODY:
            *current_state = BODY_END;
            return;
        default:
            return;
        }
    }
}
void parse_all(char *input)
{
    ParserState state = METHOD;
    printf("Start parsing...\n");
    while (state != BODY_END)
    {
        printf("DEBUG: state before lex_next = %d\n", state);
        lex_next(&input, &state);
        printf("DEBUG: state after lex_next = %d\n", state);
    }
    printf("End of parsing...\n");
}