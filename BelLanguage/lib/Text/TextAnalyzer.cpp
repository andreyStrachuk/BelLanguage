#include "TextAnalyze.h"

char *SkipSpaceSymbols (char *src) {
    assert (src);

    while (isspace (*src)) {
        src++;
    }

    return src;
}

char *SkipReadWord (char *src) {
    assert (src);

    while (*src != '\0' && *src != ' ') {
        src++;
    }

    return src;
}

int GetString (char *str) {
    assert (str);

    int size = 0;

    int symbol = getchar ();

    while (symbol != '\n') {
        str[size] = symbol;
        symbol = getchar ();
        size++;
    }

    str[size] = '\0';

    return ++size;
}

int GetFileSize (FILE *txtFile) {
    assert (txtFile);

    int numberOfSymbols = 0;

    fseek (txtFile, 0, SEEK_END);

    numberOfSymbols = ftell (txtFile);

    fseek (txtFile, 0, SEEK_SET);

    return numberOfSymbols;
}

#define ASSERT_OK(smth, doSmth)         do { if (smth) { doSmth;}} while (0)

char *GetBuffer (const char *fileName) {
    assert (fileName);

    FILE *sampleFile = fopen (fileName, "r");
    ASSERT_OK (sampleFile == nullptr, return nullptr);

    int fileSize = GetFileSize (sampleFile);

    char *buff = (char *)calloc (fileSize + 1, sizeof (char));

    fileSize = fread (buff, sizeof (char), fileSize, sampleFile);
    fclose (sampleFile);

    return buff;
}

int GetWordFromString (char *dest, char *src) {
    assert (dest);
    assert (src);
    
    int wordCounter = 0;

    if (*src == '(' || *src == ')' || *src == ';' || *src == '+' || *src == '-' || (*src == '=' && *(src + 1) != '=')) {
        dest[0] = *src;
        dest[1] = '\0';

        return ++wordCounter;
    }

    if (isalpha (*src)) {
        while (isalpha (*src)) {
            *dest = *src;
            src++;
            dest++;

            wordCounter++;
        }
    }
    else {
        while (!isspace (*src) && *src != ';' && !isdigit (*src)) {
            *dest = *src;
            src++;
            dest++;

            wordCounter++;
        }
    }

    dest [wordCounter] = '\0';

    return wordCounter;
}

int GetWord (char *dest, char **src) {
    assert (dest);
    assert (src);

    int length = 0;

    while (!isspace (**src)) {
        *dest = **src;
        dest++;
        (*src)++;

        length++;
    }

    dest [length] = '\0';

    return length;
}

#undef ASSERT_OK

void PrintErrors (const int type) {
    switch (type) {
        case UNABLETOOPENFILE: {
            printf ("Unable to open file!\nError code: %d\n", type);

            break;
        }
        case INCORRECT_INPUT: {
            printf ("Incorrect input!\nError code: %d\n", type);

            break;
        }
        default: {
            printf ("Unknown error!\nError code: %d\n", type);
        }
    }
}