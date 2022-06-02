#include "commands.h"

int GetFileSize (FILE *asmProgram) {
    assert (asmProgram);

    int numberOfSymbols = 0;

    fseek (asmProgram, 0, SEEK_END);

    numberOfSymbols = ftell (asmProgram);

    fseek (asmProgram, 0, SEEK_SET);

    return numberOfSymbols;
}

void PrintErrors (const int typeOfError) {
    switch (typeOfError) {
        case UNKNOWN_COMMAND : {
            printf ("Unknown command!\nError code: %d\n", typeOfError);
            break;
        }
        
        case UNKNOWN_REGISTER : {
            printf ("Unknown register!\nError code: %d\n", typeOfError);
            break;
        }
        case ZERO_DIV: {
            printf ("Value is divided by ZERO!\nError code: %d\n", typeOfError);
            break;
        }  
        case WRONG_ADDRESS: {
            printf ("Invalid addres!\nError code: %d\n", typeOfError);
            break;
        }
        case INCORRECT_INPUT : {
            printf ("Incorrect input!\nError code: %d\n", typeOfError);
            break;
        }
        case UNABLETOREADFROMFILE : {
            printf ("Unable to read from file!\nError code: %d\n", typeOfError);
            break;
        }
        case NULLPTR : {
            printf ("Null pointer!\nError code: %d", typeOfError);
            break;
        }

        case WRONGFILETYPE : {
            printf ("File has wrong type!\nError code: %d", typeOfError);
            break;
        }

        case UNABLETOWRITETOFILE : {
            printf ("Unable to write to file!\nError code: %d", typeOfError);
            break;
        }

        case UNABLETOOPENFILE : {
            printf ("Unable to open file!\nError code: %d", typeOfError);
            break;
        }
        case OK : {
            printf ("Everything is OK!\n");
            break;
        }

        default : {
            printf ("Unknown error!\nError number: %d\n", typeOfError);
        }
    }
}

int NumberOfStrings (FILE *asmProgram) {
    assert (asmProgram);

    int number = 0;

    char symbol = (char)getc (asmProgram);

    while (symbol != EOF) {
        if (symbol == '\n') {
            number++;
        }

        symbol = (char)getc (asmProgram);
    }

    fseek (asmProgram, 0, SEEK_SET);

    return number;
}