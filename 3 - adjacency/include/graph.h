#ifndef GRAPH
#define GRAPH

#include <stdio.h>

#ifndef BOOL
#define BOOL
typedef enum{FALSE, TRUE}bool;
#endif

#define N 11
#define V (N * N)
typedef bool adjmat[N][N];

void print_matrix(adjmat mat);
bool insert_numbers_to_matrix(adjmat mat);
#endif