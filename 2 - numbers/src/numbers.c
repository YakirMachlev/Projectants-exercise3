#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BOOL char

char *units[] = {"zero", "one", "two", "three", "four", "five", "six", "seven", "eight", "nine"};
char *tens[] = {"ten", "eleven", "twelve", "thirteen", "fourteen", "fifteen", "sixteen", "seventeen", "eighteen", "nineteen"};
char *tens_multiples[] = {"twenty", "thirty", "forty", "fifty", "sixty", "seventy", "eighty", "ninety"};

void convert_number_to_word(char *num, FILE *out)
{
    int len;
    char *digit;

    len = strlen(num);
    digit = num;

    if (len == 1)
        fprintf(out, "%s\n", units[*digit - '0']);
    else
    {
        if (*digit == '1')
            fprintf(out, "%s\n", tens[*(digit + 1) - '0']);
        else
        {
            fprintf(out, "%s ", tens_multiples[*digit - '0' - 2]);
            digit++;
            if (*digit == '0')
                fprintf(out, "\n");
            else
                fprintf(out, "%s\n", units[*digit - '0']);
        }
    }
}

void analyze_input(FILE *in, FILE *out)
{
    char num[3];
    BOOL valid;

    valid = 1;
    if (!in)
    {
        fprintf(stderr, "Read file doesn't exist\n");
        valid = 0;
    }
    if (!out)
    {        
        fprintf(stderr, "Couldn't open the write file\n");
        valid = 0;
    }
    
    if (valid)
    {
        while (fscanf(in, "%s", num) != EOF)
            convert_number_to_word(num, out);
    }

    if (in)
        fclose(in);
    if (out)
        fclose(out);
}

void handle_files(int argc, char *argv[])
{
    FILE *in;
    FILE *out;
    BOOL valid;

    valid = 1;
    switch (argc)
    {
    case 1:
        in = stdin;
        out = stdout;
        break;
    case 2:
        in = fopen(argv[1], "r");
        out = stdout;
        break;
    case 3:
        in = fopen(argv[1], "r");
        out = fopen(argv[2], "w");
        break;
    default:
        fprintf(stderr, "Too much parameters\n");
        valid = 0;
        break;
    }

    if (valid)
        analyze_input(in, out);
}

int main(int argc, char *argv[])
{
    handle_files(argc, argv);

    return 0;
}