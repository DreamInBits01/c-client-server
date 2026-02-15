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
    char *current_character_pointer = *input;
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
            return;
        case URL:
            // Second run, accumulate the URL
            while (current_character != ' ' && buffer_index < (sizeof(buffer) - 1))
            {
                buffer[buffer_index] = current_character;
                buffer_index++;
                current_character_pointer++;
                current_character = *current_character_pointer;
            }
            buffer[buffer_index] = '\0';
            printf("URL:%s\n", buffer);
            // Skip space via ++
            *input = ++current_character_pointer;
            return;
        case PROTOCOL_VERSION:
            // Third run, accumulate the PROTOCOL_VERSION
            current_character = *current_character_pointer;
            while (buffer_index < (sizeof(buffer) - 1))
            {
                current_character = *current_character_pointer;
                char ahead_character = *look_ahead(current_character_pointer);
                if (current_character == '\r' && ahead_character == '\n')
                {
                    break;
                }
                buffer[buffer_index] = current_character;
                buffer_index++;

                current_character_pointer++;
            }
            buffer[buffer_index] = '\0';
            printf("PROTOCOL_VERSION:%s\n", buffer);
            // Skip \r\n
            current_character_pointer = current_character_pointer + 2;
            *input = current_character_pointer;
            return;
        default:
            return;
        }
    }
}
void parse_all(char *input)
{
    ParserState state = METHOD;
    while (state != END_OF_REQUEST)
    {
        lex_next(&input, &state);
        // Control loop
        switch (state)
        {
        case METHOD:
            state = URL;
            break;
        case URL:
            state = PROTOCOL_VERSION;
            break;
        case PROTOCOL_VERSION:
            state = END_OF_REQUEST;
            break;
        default:
            state = END_OF_REQUEST;
            break;
        }
    }
}