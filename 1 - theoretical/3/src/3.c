#include <stdio.h>

#define STR1 'm'
#define STR2 "maaaaa"

int main()
{
    printf("%ld\n", sizeof(STR1));
    printf("%ld\n", sizeof(STR2));

    return 0;
}