#include <stdio.h>

#ifndef BOOL
#define BOOL
typedef enum{FALSE, TRUE}bool;
#endif

#define QUEUE_SIZE 30

typedef struct
{
    bool vec[QUEUE_SIZE];
    int front;
    int rear;
    int size;
}queue, * queuePtr;

void init_queue(queuePtr);
bool is_queue_empty(queuePtr);
void insert_queue(queuePtr, bool);
bool remove_queue(queuePtr);
int items_in_queue(queuePtr);
void print_queue(queuePtr);