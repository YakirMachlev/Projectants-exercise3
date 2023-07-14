#include "graph.h"

void print_matrix(adjmat mat)
{
    int row;
    int col;

    for (row = 0; row < N; row++)
    {
        for (col = 0; col < N; col++)
            printf("%d ", mat[row][col]);
        printf("\n");
    }
    printf("\n");
}

bool insert_numbers_to_matrix(adjmat mat)
{
    bool bit;
    int offset;
    bool end;

    end = 0;
    printf("Enter %d values:\n", V);

    for (offset = 0; offset < V && !end; offset++)
    {
        end = scanf("%d",  &bit) == EOF;
        mat[offset / N][offset % N] = bit;
        end |= (bit != 1 && bit != 0);
    }
    return !end;
}