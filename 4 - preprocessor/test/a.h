#ifndef DYN_INPUT
#define DYN_INPUT
#include <stdio.h>
#include <stdlib.h>

#define BUFFER_INITIAL_CAPACITY 100
#include "test//set.h"
/* #ifndef BOOLEAN
#define BOOLEAN
typedef enum{FALSE, TRUE}bool;
#endif */

void display_buffer(char *buffer);
int insert_to_buffer(char **buffer, int *length, FILE* stream);
#endif