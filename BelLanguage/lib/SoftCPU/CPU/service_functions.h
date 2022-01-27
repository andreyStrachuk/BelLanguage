#ifndef SERVICE_H
#define SERVICE_H

#include <stdio.h>
#include <assert.h>
#include <string.h>
#include <math.h>
#include "../lib/commands.h"

#include "processor.h"

#define RunProccessor(softCPU)      int res = RunCPU (softCPU);     \
                                    if (res != OK) {                \
                                        PrintError (res);           \
                                    }                               \

                                    
#define PUSHSTACK()                 PushStack (softCPU->st, val)    

#define PUSHCALLSTACK()             PushStack (softCPU->call, retVal) 

#define POPSTACK()                  PopStack (softCPU->st)

#define POPCALLSTACK()              PopStack (softCPU->call)

#define SKIPJMPVAL()                softCPU->ip += sizeof (u_int16_t)

#define GETRAMVAL(shift)            val = softCPU->RAM [shift]

#define WRITETORAM(shift)           softCPU->RAM [index] = val


double GetRegValue (const int reg, CPU *softCPU);

int WriteToRegister (int regType, CPU *softCPU, const double val);

int RunCPU (CPU *softCPU);

int CheckIfImm (const int type);

int CheckIfReg (const int type);

int CheckIfMem (const int type);

int CheckSignature (char *src);

int DetectPushnPop (char *srcName, CPU *softCPU, const int type);

#endif