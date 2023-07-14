#include <stdio.h>

#define STR1 'a'
#define STR2 "a"

int main()
{
    /* not identicle */
    printf("%ld\n", sizeof(STR1));
    printf("%ld\n", sizeof(STR2));

    /* identicle */
    printf("%c\n", STR1);
    printf("%s\n", STR2);

    return 0;
}