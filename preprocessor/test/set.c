#include "set.h"

#define BIT_CHECK(byte, nbit) (((byte) >> (nbit)) & 1)

void turn_bit_on(set *s, char offset)
{
    set mask;

    mask = 1;
    mask <<= offset;
    *s |= mask;
}

void read_set(set *curSet, char *members)
{
    *curSet = 0;

    /* printf("Entered read_set\n"); */

    while (*members != -1)
    {
        /* printf("%d ", *members); */
        turn_bit_on(curSet, *members);
        members++;
    }
    printf("\n");
}

void print_set(set curSet)
{
    char offset;
    int countMembers;
    /* printf("Entered print_set\n"); */

    countMembers = 1;
    if (curSet)
    {
        printf("The set members are:\n");
        for (offset = 0; offset < sizeof(curSet) * 8; offset++)
        {
            if (BIT_CHECK(curSet, offset))
            {
                printf("%d ", offset);
                countMembers++;
            }
            if (!(countMembers % 17))
            {
                countMembers = 1;
                printf("\n");
            }
        }
    }
    else
        printf("The set is empty\n");
    /* printf("\n"); */
}

void union_set(set set1, set set2, set *setTo)
{
    *setTo = set1 | set2;
}

void intersect_set(set set1, set set2, set *setTo)
{
    *setTo = set1 & set2;
}

void sub_set(set set1, set set2, set *setTo)
{
    *setTo = (set1 ^ set2) & set1;
}

void symdiff_set(set set1, set set2, set *setTo)
{
    *setTo = set1 ^ set2;
}