#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <search.h>
#include "dynamicInput.h"

#define MAX_AMOUNT_OF_MODULES 256
void preprocess_file(char*);
typedef enum{FALSE, TRUE}bool;

FILE* open_file_r(char* fileName)
{
    FILE *f;

    f = fopen(fileName, "r");
    if (!f)
        printf("The file %s doesn't exist\n", fileName);
    
    return f;
}

FILE* open_file_w(char* fileName)
{
    FILE *f;

    f = fopen(fileName, "w");
    if (!f)
        printf("Couldn't create the file %s\n", fileName);
    
    return f;
}

char* append_char_to_string(char* str, char suffix)
{
    char* newStr;
    unsigned char strLen;

    strLen = strlen(str);
    newStr = (char*)malloc(sizeof(char) * (strLen + 2));
    strcpy(newStr, str);
    newStr[strLen] = suffix;
    newStr[strLen + 1] = '\0';
    return newStr;
}

char* extract_header_name(char* line)
{
    int fileNameLength;
    char* closingQuote;
    char* includeFileName;

    includeFileName = NULL;
    closingQuote = strchr(line + 10, '\"');
    if (closingQuote)
    {
        fileNameLength = closingQuote - (line + 10);
        includeFileName = (char*)malloc((fileNameLength + 1) * sizeof(char));
        strncpy(includeFileName, line + 10, fileNameLength);
        includeFileName[fileNameLength] = '\0';        
    }
    return includeFileName;
}


void remove_comments(char* inputFileName, char* outputFileName)
{
    char currentChar;
    char nextChar;
    FILE* inputFile;
    FILE* outputFile;
    bool commentEnd;

    inputFile = open_file_r(inputFileName);
    outputFile = open_file_w(outputFileName);

    if (inputFile && outputFile)
    {
        while ((currentChar = fgetc(inputFile)) != EOF)
        {
            if (currentChar == '/' && (nextChar = fgetc(inputFile)) == '*')
            {
                commentEnd = 0;
                while ((currentChar = fgetc(inputFile)) != EOF && !commentEnd)
                {
                    if (currentChar == '*' && (nextChar = fgetc(inputFile)) == '/')
                        commentEnd = 1;
                }
            }

            fputc(currentChar, outputFile);
        }
    }

    fclose(inputFile);
    fclose(outputFile);
}

void remove_comments_wrapper(char* inputFileName)
{
    char* withoutComments;
    withoutComments = append_char_to_string(inputFileName, '1');
    remove_comments(inputFileName, withoutComments);
    free(withoutComments);
}

void copy_file_to_file(char* includeFileName, char* includeLine, int includeLineSize, FILE* outputFile)
{
    FILE* includeFile;

    includeFile = open_file_r(includeFileName);
    if (includeFile)
    {
        while (insert_to_buffer(&includeLine, &includeLineSize, includeFile) != -1)
        {
            fputs(includeLine, outputFile);
            fputc('\n', outputFile);
        }
        fclose(includeFile);
    }
    else
        printf("Couldn't find the file \"%s\"\n", includeFileName);
}

bool identicle_strings_until_char(char* str1, char* str2, char saparator)
{
    char* curr1;
    char* curr2;
    char temp1;
    char temp2;
    bool identicle;

    curr1 = str1;
    curr2 = str2;
    while (*(curr1++) != saparator);
    while (*(curr2++) != saparator);
    temp1 = *curr1;
    temp2 = *curr2;
    *curr1 = '\0';
    *curr2 = '\0';
    identicle = !strcmp(str1, str2);
    *curr1 = temp1;
    *curr2 = temp2;

    return identicle;
}

void analyze_lines_headers_to_file(char* line, char* includeLine, int includeLineSize, FILE* outputFile, char* callerName)
{
    char* includeFileName;
    int len;
    char* modifiedFileName;

    if (!strncmp(line, "#include \"", 10))
    {
        includeFileName = extract_header_name(line);
        if (includeFileName)
        {
            len = strlen(includeFileName);
            modifiedFileName = append_char_to_string(includeFileName, '2');
            if (identicle_strings_until_char(includeFileName, callerName, '.'))
            {
                preprocess_file(includeFileName);
                copy_file_to_file(modifiedFileName, includeLine, includeLineSize, outputFile);
            }
            else
            {
                includeFileName[len - 1] = 'c';
                preprocess_file(includeFileName);
                modifiedFileName[len - 1] = 'c';
                copy_file_to_file(modifiedFileName, includeLine, includeLineSize, outputFile);
            }

            free(includeFileName);
            free(modifiedFileName);
        }
        else
            printf("Invalid #include\n");
    }
    else
    {
        fputs(line, outputFile);
        fputc('\n', outputFile);
    }
}

void insert_headers_to_file(char* inputFileName, char* outputFileName, char* callerName)
{
    FILE* inputFile;
    FILE* outputFile;
    char* line;
    int lineSize;
    char* includeLine;
    int includeLineSize;

    inputFile = open_file_r(inputFileName);
    outputFile = open_file_w(outputFileName);

    if (inputFile && outputFile)
    {
        line = (char*)malloc(sizeof(char) * BUFFER_INITIAL_CAPACITY);
        lineSize = BUFFER_INITIAL_CAPACITY;
        includeLine = (char*)malloc(sizeof(char) * BUFFER_INITIAL_CAPACITY);
        includeLineSize = BUFFER_INITIAL_CAPACITY;
        
        while (insert_to_buffer(&line, &lineSize, inputFile) != -1)
            analyze_lines_headers_to_file(line, includeLine, includeLineSize, outputFile, callerName);
        
        free(line);
        free(includeLine);
    }

    fclose(inputFile);
    fclose(outputFile);
}

void insert_headers_to_file_wrapper(char* inputFileName)
{
    char* withoutComments;
    char* withoutHeaders;
    
    withoutComments = append_char_to_string(inputFileName, '1');
    withoutHeaders = append_char_to_string(inputFileName, '2');
    insert_headers_to_file(withoutComments, withoutHeaders, inputFileName);
    
    free(withoutComments);
    free(withoutHeaders);
}

void preprocess_file(char* fileName)
{
    ENTRY e, *ep;

    printf("current file: %s\n", fileName);
    e.key = fileName;
    e.data = (void*)1;
    ep = hsearch(e, FIND);
    if (!ep)
    {
        ep = hsearch(e, ENTER);
        remove_comments_wrapper(fileName);
        printf("created file %s1\n", fileName);
        
        insert_headers_to_file_wrapper(fileName);
        printf("created file %s2\n", fileName);        
    }
    else
        printf("header storm detected\n");
}

int main(int argc, char* argv[])
{
    hcreate(MAX_AMOUNT_OF_MODULES);
    
    if (argc == 2)
        preprocess_file(argv[1]);
    else
        printf("Invalid parameters\n");


    return 0;
}