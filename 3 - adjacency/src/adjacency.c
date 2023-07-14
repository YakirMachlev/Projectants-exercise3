#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include "queue.h"
#include "graph.h"

#ifndef BOOL
#define BOOL
typedef enum{FALSE, TRUE}bool;
#endif
#define IS_TERMINAL isatty(0)

bool path(adjmat adjacencyMat, unsigned char index1, unsigned char index2)
{
    bool visited[V] = {0};
    int offset;
    unsigned char curIndex;
    queue q;
    queuePtr qp;
    
    if (index1 < 0 || index1 >= N || index2 < 0 || index2 >= N)
        return FALSE;
    if (index1 == index2)
        return TRUE;

    qp = &q;
    init_queue(qp);
    visited[index1] = TRUE;
    
    insert_queue(qp, index1); 
    while (!is_queue_empty(qp))
    {
        curIndex = remove_queue(qp);

        for (offset = 0; offset < N; offset++)
        {
            if (curIndex == index2)
                return TRUE;
 
            if (adjacencyMat[curIndex][offset] && !visited[offset])
            {
                visited[offset] = TRUE;
                insert_queue(qp, offset);
            }
        }
    }
    return FALSE;
}

/* works only if each node has a single father */
bool path_backwards(adjmat adjacencyMat, unsigned char index1, unsigned char index2)
{
    unsigned char offset;
    unsigned char son;
    unsigned char count;

    if (index1 < 0 || index1 >= N || index2 < 0 || index2 >= N)
        return FALSE;
    if (index1 == index2)
        return TRUE;
    
    count = 1;
    son = index2;
    while (count)
    {
        count = 0;
        for (offset = 0; offset < N && !count; offset++)
        {
            if (son == index1)
                return TRUE;
            count += adjacencyMat[offset][son];
            if (adjacencyMat[offset][son])
                son = offset;
        }
    }
    return FALSE;
}

void check_connections(adjmat mat)
{
    unsigned char index1;    
    unsigned char index2;
    bool end;
    bool path_exists;

    if (IS_TERMINAL)
        printf("\nEnter the first index: ");
    end = scanf("%d", &index1) == EOF;
    if (IS_TERMINAL)
        printf("Enter the second index: ");
    end |= scanf("%d", &index2) == EOF;

    while ((index1 != -1 || index2 != -1) && !end)
    {
        path_exists = path_backwards(mat, index1, index2);
        if (path_exists)
            printf("A path exists between the nodes %d->%d\n", index1, index2);
        else
            printf("There isn't a path between the nodes %d->%d\n", index1, index2);
        
        if (IS_TERMINAL)
            printf("\nEnter the first index: ");
        end = scanf("%d", &index1) == EOF;
        if (IS_TERMINAL)
            printf("Enter the second index: ");
        end |= scanf("%d", &index2) == EOF;
    }
}

int main()
{
    adjmat adjacencyMat;

    printf("The matrix is a %d by %d matrix\n", N, N);
    if (insert_numbers_to_matrix(adjacencyMat))
    {
        print_matrix(adjacencyMat);
        check_connections(adjacencyMat);
    }

    return 0;
}