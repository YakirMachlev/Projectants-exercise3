#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include "dynamicInput.h"

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

void analyze_lines_headers_to_file(char* line, char* includeLine, int includeLineSize, FILE* outputFile)
{
    char* includeFileName;

    if (!strncmp(line, "#include \"", 10))
    {
        includeFileName = extract_header_name(line);
        if (includeFileName)
        {
            copy_file_to_file(includeFileName, includeLine, includeLineSize, outputFile);
            preprocess_file(includeFileName); /* recursion */
            
            includeFileName[strlen(includeFileName) - 1] = 'c';
            preprocess_file(includeFileName); /* recursion */

            copy_file_to_file(includeFileName, includeLine, includeLineSize, outputFile);
            free(includeFileName);
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

void insert_headers_to_file(char* inputFileName, char* outputFileName)
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
        {
            analyze_lines_headers_to_file(line, includeLine, includeLineSize, outputFile);
        }        
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
    insert_headers_to_file(withoutComments, withoutHeaders);
    
    free(withoutComments);
    free(withoutHeaders);
}

void preprocess_file(char* fileName)
{
    remove_comments_wrapper(fileName);
    insert_headers_to_file_wrapper(fileName);
}

int main(int argc, char* argv[])
{
    if (argc == 2)
    {   
        preprocess_file(argv[1]);
    }
    else
        printf("Invalid parameters\n");

    return 0;
}