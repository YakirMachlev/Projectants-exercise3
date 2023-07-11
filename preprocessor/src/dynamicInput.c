#include "dynamicInput.h"

void display_buffer(char *buffer)
{
    while (*buffer)
        printf("%c", *(buffer++));
    printf("\n");
}

char *insert_to_buffer(char *buffer, int *length)
{
    int capacity;
    char tav;
    BOOL flag;
    char *tempBuffer;

    capacity = BUFFER_INITIAL_CAPACITY;
    *length = 0;
    flag = 1;

    while ((tav = getchar()) != '\n' && tav != EOF && flag)
    {
        buffer[(*length)++] = tav;
        if (*length >= capacity)
        {
            capacity += BUFFER_INITIAL_CAPACITY / 2;
            tempBuffer = (char *)realloc(buffer, capacity * sizeof(char));
            if (tempBuffer)
                buffer = tempBuffer;
            else
            {
                printf("Line too long. Reallocation failed\n");
                flag = 0;
            }
        }
    }
    buffer[*length] = '\0';

    return buffer;
}