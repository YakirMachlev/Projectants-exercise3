#include <stdio.h>
#include "hello.h"

/*
    Function to calculate the factorial
    This function uses recursion to calculate the factorial of a given number.
    Parameters:
        n: The number for which factorial is to be calculated.
    Returns:
        The factorial of the given number.
*/
int factorial(int n)
{
    if (n == 0 || n == 1)
    {
        return 1;
    }
    else
    {
        /* Recursive call to calculate the factorial */
        return n * factorial(n - 1);
    }
}

int main()
{
    int number;

    printf("Enter a number: ");
    scanf("%d", &number);

    /* Calculate the factorial of the number */
    int result = factorial(number);

    printf("The factorial of %d is: %d\n", number, result);

    /*
        Display a pattern
        This section prints a pattern of asterisks based on the input number.
    */
    printf("Pattern:\n");
    for (int i = 1; i <= number; i++)
    {
        /* Print spaces */
        for (int j = 1; j <= number - i; j++)
        {
            printf(" ");
        }

        /* Print asterisks */
        for (int k = 1; k <= i; k++)
        {
            printf("* ");
        }

        printf("\n");
    }

    return 0;
}
