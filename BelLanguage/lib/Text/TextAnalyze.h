#pragma once

#include <stdio.h>
#include <assert.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>

#define STR_EQ(str1, str2)              (strncmp (str1, str2, strlen (str1)) == 0)

enum ERRORS {
    OK = -11,
    UNABLETOOPENFILE = -10,
    INCORRECT_INPUT = -9,
    NULLPTR = -8,
};

char *SkipSpaceSymbols (char *src);

char *SkipReadWord (char *src);

int GetString (char *str);

int GetFileSize (FILE *sample);

int GetBuffer (char **buff, const char *fileName);

void PrintErrors (const int type);

int GetWordFromString (char *dest, char *src);