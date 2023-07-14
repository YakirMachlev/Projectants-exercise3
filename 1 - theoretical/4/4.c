#include <stdio.h>

void func_by_value(int num)
{
    num = 50;
}

void func_by_address(int* num)
{
    *num = 50;
}

int main()
{
    int num;
    num = 25;
    printf("1. num: %d\n", num);

    func_by_value(num);
    printf("2. num: %d\n", num);

    func_by_address(&num);
    printf("3. num: %d\n", num);

    return 0;
}