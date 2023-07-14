#include <stdio.h>
#include <stdlib.h>

int main(int argc, char* argv[])
{
    int num;

    /* $ ./7 < file.txt - redirection of stdin
       $ ./7 > file.txt - redirection of stdout
       $ ./7 2> file.txt - redirection of stderr*/
    fprintf(stderr, "hello\n");
    printf("bye\n");
    scanf("%d", &num);

    return 0;
}