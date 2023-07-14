#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <search.h>
#include "dynamicInput.h"

#define MAX_AMOUNT_OF_MODULES 256
void preprocess_file(char*);
#ifndef BOOL
#define BOOL
typedef enum{FALSE, TRUE}bool;
#endif

/**
 * @brief creates a copy of the given string and appends a single given
 * character to the end of the string
 * 
 * @param str a string
 * @param suffix the character that we append
 * @return char* a copy of the string with the suffix appended
 */
char* append_char_to_string(char* str, char suffix)
{
    char* newStr;
    unsigned char strLen;

    strLen = strlen(str);
    newStr = (char*)malloc(sizeof(char) * (strLen + 1));
    strcpy(newStr, str);
    newStr[strLen] = suffix;
    newStr[strLen + 1] = '\0';
    return newStr;
}

/**
 * @brief iterates over a file and copies to an output file the entire content
 * of the input file except for the comments
 * 
 * @param inputFileName the input file
 * @param outputFileName the output file
 */
void remove_comments(char* inputFileName, char* outputFileName)
{
    char currentChar;
    char nextChar;
    FILE* inputFile;
    FILE* outputFile;
    bool commentEnd;

    inputFile = fopen(inputFileName, "r");
    outputFile = fopen(outputFileName, "w");

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
    else
        printf("Error when opening the files\n");

    fclose(inputFile);
    fclose(outputFile);
}

/**
 * @brief creates the name for the output file used in the remove_comments
 * function, and calls it
 * 
 * @param inputFileName the name of the input file, that is used as the base
 * for the output file name
 */
void remove_comments_wrapper(char* inputFileName)
{
    char* withoutComments;
    withoutComments = append_char_to_string(inputFileName, '1');
    remove_comments(inputFileName, withoutComments);
    free(withoutComments);
}

/**
 * @brief extracts the name of the header from a line that contains an
 * #include statement
 * 
 * @param line the line that from which it extract the header
 * @return char* the header name
 */
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

/**
 * @brief checks if two strings are identicle until they reach a given
 * character
 * 
 * @param str1 the first string
 * @param str2 the second string
 * @param separator the character that acts as the separator
 * @return true when the string are identicle till a certain char
 * @return false when the string aren't identicle till a certain char
 */
bool identicle_strings_until_char(char* str1, char* str2, char separator)
{
    char* curr1;
    char* curr2;
    char temp1;
    char temp2;
    bool identicle;

    curr1 = str1;
    curr2 = str2;
    while (*(curr1++) != separator);
    while (*(curr2++) != separator);
    temp1 = *curr1;
    temp2 = *curr2;
    *curr1 = '\0';
    *curr2 = '\0';
    identicle = !strcmp(str1, str2);
    *curr1 = temp1;
    *curr2 = temp2;

    return identicle;
}

/**
 * @brief copies all the content of the included file into the given output file
 * 
 * @param includeFileName the name of the file that is included
 * @param includeLine used to store each line from the included file
 * @param includeLineSize the max size of a line in the included file
 * @param outputFile the file that the function writes into
 */
void copy_file_to_file(char* includeFileName, char* includeLine, int includeLineSize, FILE* outputFile)
{
    FILE* includeFile;

    includeFile = fopen(includeFileName, "r");
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

/**
 * @brief checks if a line contains an #include statement. If so, it will recursively preprocess
 * the header file. Else, it will copy the line to the given file.
 * 
 * @param line the analyzed line
 * @param includeLine used to store each line from the included file
 * @param includeLineSize the max size of a line in the included file
 * @param outputFile the file that the function writes into
 * @param callerName the name of the file that contains the include statments
 */
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

/**
 * @brief copies from the given input file the entire text written in the #include statements
 * (recursively) into the given output file
 * 
 * @param inputFileName the input file
 * @param outputFileName the output file
 * @param callerName the name of the file that contains the include statements
 */
void insert_headers_to_file(char* inputFileName, char* outputFileName, char* callerName)
{
    FILE* inputFile;
    FILE* outputFile;
    char* line;
    int lineSize;
    char* includeLine;
    int includeLineSize;

    inputFile = fopen(inputFileName, "r");
    outputFile = fopen(outputFileName, "w");

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
    else
        printf("Error when opening the files\n");
    
    fclose(inputFile);
    fclose(outputFile);
}

/**
 * @brief creates the name for the output file used in the insert_headers_to_file
 * function, and calls it
 * 
 * @param inputFileName the name of the input file, that is used as the base
 * for the output file name
 */
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

/**
 * @brief creates fileName.c1, where all of the comments are removed, and
 * also fileName.c2 that is based on fileName.c1, where the entire text
 * written in the #include statements are copied into the file.
 * 
 * @param fileName the fileName that it preprocesses
 */
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