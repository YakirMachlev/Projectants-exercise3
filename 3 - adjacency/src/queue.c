#include "queue.h"

void init_queue(queuePtr que)
{
    que->front = 0;
    que->rear = 0;
    que->size = 0;
}

bool is_queue_empty(queuePtr que)
{
    return que->size == 0;
}

void insert_queue(queuePtr que, bool item)
{
    que->vec[que->rear++] = item;
    que->rear = (que->rear) % QUEUE_SIZE;
    que->size++;
}

bool remove_queue(queuePtr que)
{
    bool item;
    item = que->vec[que->front++];
    que->front = (que->front) % QUEUE_SIZE;
    que->size--;
    return item;
}

int items_in_queue(queuePtr que)
{
    return que->size;
}

void print_queue(queuePtr que)
{
    while (!is_queue_empty(que))
    {
        printf("%d", remove_queue(que));
    }
}