#ifndef GENERAL_FUNCTIONS_H
#define GENERAL_FUNCTIONS_H

#include <stdio.h>
#include <assert.h>
#include <string.h>
#include <ctype.h>
#include <math.h>
#include <stdlib.h>

#include "../lib/commands.h"   

#define FindNextWord(src)                       src = SkipSpaceSymbols (src);                                                                                                   \
                                                if (*src == '\0' || *src == ';') continue                                                                                       \

//!TODO bit masks?
#define FillBitField(field, a, command, type)   field.imm = 0;                                                                                                                  \
                                                field.reg = 0;                                                                                                                  \
                                                field.mem = 0;                                                                                                                  \
                                                if (type == IMM) { field.imm = 1;}                                                                                              \
                                                else if (type == REG) { field.reg = 1; }                                                                                        \
                                                else if (type == MEM) { field.mem = 1; }                                                                                        \
                                                else if (type == MEMREG) { field.mem = 1; field.reg = 1;}                                                                       \
                                                else if (type == MEMREGIMM) { field.mem = 1; field.reg = 1; field.imm = 1; }                                                    \
                                                field.cmd = command;                                                                                                            \
                                                a = field.cmd + field.imm * (2 * 2 * 2 * 2 * 2) + field.reg * (2 * 2 * 2 * 2 * 2 * 2) + field.mem * (2 * 2 * 2 * 2 * 2 * 2 * 2) \

#define ASSERT_CORRECT(src)                     src = SkipSpaceSymbols (src);                                                                                                   \
                                                if (*src != ';' && *src != '\0') { return INCORRECT_INPUT; }

#define CheckLabel(src, lbls, labelIp, sizeOfCodeArr)   int res = CheckIfLabel (src, lbls, labelIp, sizeOfCodeArr);                                                             \
                                                        printf ("labelIp = %d\ni = %d\n", res, *labelIp);                                                                       \
                                                        if (res == OK) {                                                                                                        \
                                                            lbls[*labelIp].ip = sizeOfCodeArr;                                                                                  \
                                                            (*labelIp)++;                                                                                                       \
                                                            continue;                                                                                                           \
                                                        }

#define ReadCmdsAndWrite(code, cmds, numOfStrings, lbls, labelIp)   result = ReadCmdAndWrite (code, cmds, numOfStrings, lbls, FIRST, &labelIp);                                 \
                                                                    ASSERT_OKAY (result != OK, PrintErrors (result);)                                                           \
                                                                    result = ReadCmdAndWrite (code, cmds, numOfStrings, lbls, SECOND, &labelIp);                                \
                                                                    ASSERT_OKAY (result != OK, PrintErrors (result);)                                                           \
                                                                    

#define DetectReg(reg)          typeOfReg = DetectRegister (reg);                                                                                                               \
                                if (typeOfReg == UNKNOWN_REGISTER) {                                                                                                            \
                                    return UNKNOWN_REGISTER;                                                                                                                    \
                                }

#define ARRANGEPUSHANDPOPCMDS()             int check = ArrangePushAndPopCmd (src, typeOfCmd, machineCode, &sizeOfCodeArr); \
                                            if (check == OK) continue;                                                      \
                                            else return check; 

#define ARRANGEJMPS()           int check = ArrangeJmpsCall (src, typeOfCmd, machineCode, &sizeOfCodeArr, lbls, labelIp); \
                                if (check == OK) continue;                                                              \
                                else return check;
        

struct Line {
    char *str;
    int length;
};

char *SkipSpaceSymbols (char *src);

char *SkipReadWord (char *src);

int DetectCommand (char *src);

int DetectRegister (char *src);

char *SkipArg (char *src);

int ReadCmdAndWrite (FILE *code, Line *cmds, const int numbOfStrings,Label *lbls, const int typeOfCheck, int *labelIp);

void InitializeArrOfPointers (Line *cmds, char *src, const int numberOfStrings);

void PutDouble (const double value, char *ptr, int *sizeOfArr);

void PutInt (const int value, char *ptr, int *sizeOfArr);

int CheckIfLabel (char *src, Label *lbls, int *labelIp, const int sizeOfCodeArr);

Line GetLine (char *src);

int CheckIfLabelContainsStr (Label *lbls, char *src, const int labelIp);

void MemFree (char *asmProg, Line *cmds, const int numOfStrings, Label *lbls, const int labelIp);

int ArrangePushAndPopCmd (char *src, const int typeOfCmd, char *machineCode, int *sizeOfCodeArr);

int ArrangeJmpsCall (char *src, const int typeOfCmd, char *machineCode, int *sizeOfCodeArr, Label *lbls, int *labelIp);

#endif