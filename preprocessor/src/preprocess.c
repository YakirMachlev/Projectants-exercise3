#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

typedef enum{FALSE, TRUE}bool;

int getline(char **lineptr, int *n, FILE *stream)
{
    static char line[256];
    char *ptr;
    unsigned int len;

    if (lineptr == NULL || n == NULL)
    {
        errno = EINVAL;
        return -1;
    }

    if (ferror (stream))
        return -1;

    if (feof(stream))
        return -1;

    fgets(line,256,stream);

    ptr = strchr(line,'\n');   
    if (ptr)
        *ptr = '\0';

    len = strlen(line);

    if ((len+1) < 256)
    {
        ptr = realloc(*lineptr, 256);
        if (ptr == NULL)
            return(-1);
        *lineptr = ptr;
        *n = 256;
    }

    strcpy(*lineptr,line); 
    return(len);
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

void remove_comments(char* in, char* out)
{
    char currentChar;
    char nextChar;
    FILE* original;
    FILE* modified;
    bool commentEnd;

    original = open_file_r(in);
    modified = open_file_w(out);

    while ((currentChar = fgetc(original)) != EOF)
    {
        if (currentChar == '/' && (nextChar = fgetc(original)) == '*')
        {
            commentEnd = 0;
            while ((currentChar = fgetc(original)) != EOF && !commentEnd)
            {
                if (currentChar == '*' && (nextChar = fgetc(original)) == '/')
                    commentEnd = 1;
            }
        }

        fputc(currentChar, modified);
    }

    fclose(original);
    fclose(modified);
}

void insert_headers_to_file(char* in, char* out)
{
    FILE* original;
    FILE* modified;
    char* line;
    int lineSize;
    char* includeFileName;
    FILE* includeFile;
    char* includeLine;
    int includeLineSize;

    original = open_file_r(in);
    modified = open_file_w(out);

    line = NULL;
    lineSize = 0;

    if (original && modified)
    {
        while (getline(&line, &lineSize, original) != -1)
        {
            if (strncmp(line, "#include \"", 10) == 0)
            {
                includeFileName = extract_header_name(line);
                if (includeFileName)
                {
                    includeFile = open_file_r(includeFileName);
                    if (!includeFile)
                    {
                        fclose(original);
                        fclose(modified);
                        free(line);
                        free(includeFileName);
                        return;
                    }

                    includeLine = NULL;
                    includeLineSize = 0;
                    while (getline(&includeLine, &includeLineSize, includeFile) != -1)
                    {
                        fputs(includeLine, modified);
                        fputc('\n', modified);
                    }

                    fclose(includeFile);
                    free(includeLine);
                    free(includeFileName);
                }
            }
            else
            {
                fputs(line, modified);
                fputc('\n', modified);
            }
        }        
    }

    fclose(original);
    fclose(modified);
    free(line);
}

int main(int argc, char* argv[])
{
    char* withoutComments;
    char* withoutHeaders;

    withoutComments = append_char_to_string(argv[1], '1');
    withoutHeaders = append_char_to_string(argv[1], '2');

    remove_comments(argv[1], withoutComments);
    insert_headers_to_file(withoutComments, withoutHeaders);

    free(withoutComments);
    free(withoutHeaders);

    return 0;
}