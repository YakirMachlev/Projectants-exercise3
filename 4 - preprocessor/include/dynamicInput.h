#ifndef DYN_INPUT
#define DYN_INPUT
#include <stdio.h>
#include <stdlib.h>

#define BUFFER_INITIAL_CAPACITY 100

#ifndef BOOL
#define BOOL
typedef enum{FALSE, TRUE}bool;
#endif

/**
 * @brief prints a given buffer
 * 
 * @param buffer a buffer
 */
void display_buffer(char *buffer);

/**
 * @brief inserts a line from stream to buffer dynamically
 * 
 * @param buffer a pointer to the buffer
 * @param length the length of the buffer
 * @param stream the file that it iterates over
 * @return int 
 */
int insert_to_buffer(char **buffer, int *length, FILE* stream);
#endif