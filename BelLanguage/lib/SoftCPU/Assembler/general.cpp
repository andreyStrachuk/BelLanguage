#include "general_functions.h"

char *SkipSpaceSymbols (char *src) {
    assert (src);

    while (*src == ' ' && (*src != '\0' || *src != ';')) {
        src++;
    }

    return src;
}

char *SkipReadWord (char *src) {
    assert (src);

    while (*src != '\0' && *src != ' ' && *src != ';') {
        src++;
    }

    return src;
}

static int MakeSignature (FILE *code, const char *src) {
    ASSERT_OKAY (code == nullptr, return NULLPTR);
    ASSERT_OKAY (code == nullptr, return NULLPTR);

    fwrite (src, sizeof (char), 2, code);

    return OK;
}

#define DEF_CMD_(name, number, argNumber, code, function)  if (STR_EQ (src, #name)) { \
                                    return CMD_##name; \
                                } \

int DetectCommand (char *src) {
    assert (src);

    #include "commands_def.h"

    #undef DEF_CMD_

    return UNKNOWN_COMMAND;
}

#define DEF_REG_(name, number)  if (STR_EQ (#name, src)) { \
                                    return R_##name; \
                                } \

int DetectRegister (char *src) {
    assert (src);

    #include "regs_def.h"

    #undef DEF_REG_

    return UNKNOWN_REGISTER;
}

char *SkipArg (char *src) {
    assert (src);

    while (*src != ' ' && *src != '\n' && *src != '\0') src++;

    return src;
}

#define DEF_CMD_(name, cmdNumb, argNumber, code, function)                                                          \
            if (typeOfCmd == cmdNumb) {                                                                             \
                src = SkipSpaceSymbols (src);                                                                       \
                                                                                                                    \
                switch (argNumber) {                                                                                \
                    case 0: {                                                                                       \
                        ASSERT_OKAY (*src != ';' &&  *src != '\0', return INCORRECT_INPUT;)                         \
                                                                                                                    \
                        FillBitField (cmdNum, cmdNumber, typeOfCmd, NTHG);                                          \
                                                                                                                    \
                        machineCode [sizeOfCodeArr++] = cmdNumber;                                                  \
                        continue;                                                                                   \
                    }                                                                                               \
                                                                                                                    \
                    case 1: {                                                                                       \
                                                                                                                    \
                            function                                                                                \
                            continue;                                                                               \
                    }                                                                                               \
                    case 2: {                                                                                       \
                        FindNextWord (src);                                                                         \
                        if (*src != ':') return INCORRECT_INPUT;                                                    \
                                                                                                                    \
                        function                                                                                    \
                                                                                                                    \
                        continue;                                                                                   \
                    }                                                                                               \
                    default : {                                                                                     \
                        return INCORRECT_INPUT;                                                                     \
                    }                                                                                               \
                }                                                                                                   \
            }


int ReadCmdAndWrite (FILE *code, Line *cmds, const int numbOfStrings, Label *lbls, const int typeOfCheck, int *labelIp) {
    assert (code);
    assert (cmds);

    char commands [10] = {};
    char *machineCode = (char *)calloc (SIZEOFCODEARR, sizeof (char));
    int sizeOfCodeArr = 0;
    char reg [10] = {};
    unsigned char cmdNumber = 0;
    double arg = 0;
    struct COMMAND cmdNum = {};
    int typeOfReg = 0;
    int resOfScan = 0;
    int typeOfCmd = 0;

    for (int i = 0; i < numbOfStrings; i++) {
        char *src = cmds[i].str;

        FindNextWord (src);

        int res = CheckIfLabel (src, lbls, labelIp, sizeOfCodeArr);
        ASSERT_OKAY (res == OK, continue; )

        resOfScan = sscanf (src, "%s", commands);
        typeOfCmd = DetectCommand (commands);

        ASSERT_OKAY (typeOfCmd == UNKNOWN_COMMAND, return UNKNOWN_COMMAND;)

        src = SkipReadWord (src);

        #include "commands_def.h"

        #undef DEF_CMD_

    }

    if (typeOfCheck == SECOND) {
        MakeSignature (code, SIGN);

        fwrite (machineCode, sizeof (char), sizeOfCodeArr, code);
    }

    free (machineCode);

    return OK;
}

Line GetLine (char *src) {
    assert (src);

    Line cmdLine = {};

    cmdLine.str = src;

    while (*src != '\n') {
        cmdLine.length++;
        src++;
    }

    *src = '\0';
    cmdLine.length++;

    return cmdLine;
}

void InitializeArrOfPointers (Line *cmds, char *src, const int numberOfStrings) {
    assert (cmds);
    assert (src);

    for (int i = 0; i < numberOfStrings; i++) {
        cmds[i] = GetLine (src);

        src += cmds[i].length;
    }
}

void PutDouble (const double value, char *ptr, int *sizeOfArr) {
    char *p = (char *)(&value);

    for (int i = 0; i < (int)sizeof value; i++) {
        *(ptr + *sizeOfArr) = *p;
        p++;
        (*sizeOfArr)++;
    }
}

void PutInt (const int value, char *ptr, int *sizeOfArr) {
    char *p = (char *)(&value);

    for (int i = 0; i < 2; i++) {
        *(ptr + *sizeOfArr) = *p;
        p++;
        (*sizeOfArr)++;
    }
}

int CheckIfLabel (char *src, Label *lbls, int *labelIp, const int sizeOfCodeArr) {
    if (*src == ':') {
        src++;
        char str[10] = {0};
        int n = 0;

        sscanf (src, "%s%n", str, &n);

        int res = CheckIfLabelContainsStr (lbls, str, *labelIp);
        if (res != -1) {
            return OK;
        }

        (lbls + *labelIp)->txt = (char *)calloc (n, sizeof (char)); 

        strncpy ((*(lbls + *labelIp)).txt, str, n);

        (*(lbls + *labelIp)).ip = sizeOfCodeArr;

        (*labelIp)++;

        return OK;
    }

    return 0;
}

int CheckIfLabelContainsStr (Label *lbls, char *src, const int labelIp) {
    assert (lbls);
    assert (src);

    int dest = -1;

    for (int i = 0; i < labelIp; i++) {
        if (strncmp (lbls[i].txt, src, strlen (src)) == 0) {
            dest = i;
            break;
        } 
    }

    return dest;
}

void MemFree (char *asmProg, Line *cmds, int numOfStrings, Label *lbls, const int labelIp) {
    assert (asmProg);
    assert (cmds);
    assert (lbls);

    for (int i = 0; i < labelIp; i++) {
        free (lbls[i].txt);
    }

    free (cmds);
    free (lbls);
    free (asmProg);
}

int ArrangePushAndPopCmd (char *src, const int typeOfCmd, char *machineCode, int *sizeOfCodeArr) { //!TODO rename
  
    double arg = 0;
    int resOfScan = sscanf (src, "%lg", &arg); 
    struct COMMAND field = {};    
    unsigned char cmdNumber = 0;       
    char reg[5] = {};
    int typeOfReg = 0;

    if (typeOfCmd == CMD_pop && (*src == ';' ||  *src == '\0')) {
        FillBitField (field, cmdNumber, typeOfCmd, IMM);
        machineCode [(*sizeOfCodeArr)++] = cmdNumber;
        return OK;
    }                           
                                                                                                                    
    if (resOfScan != 0) {                                                                   
        src = SkipReadWord (src);                                                           
        ASSERT_CORRECT (src);                                                               
                                                                                                                    
        FillBitField (field, cmdNumber, typeOfCmd, IMM);                                   
                                                                                                                    
        machineCode [(*sizeOfCodeArr)++] = cmdNumber;                                          
                                                                                                                    
        PutDouble (arg, machineCode, sizeOfCodeArr);

        return OK;                                                                   
    }

    int shift = 0;
    int ok = 0;
    resOfScan = sscanf (src, "[%d]%n", &shift, &ok);                         
                                                                                                    
    if (ok >= 3) {                                                                          
        src = SkipReadWord (src);                                                           
        ASSERT_CORRECT (src);                                                               
                                                                                            
        FillBitField (field, cmdNumber, typeOfCmd, MEM);                                  
                                                                                            
        machineCode[(*sizeOfCodeArr)++] = cmdNumber;                                          
        PutInt (shift, machineCode, sizeOfCodeArr);    

        return OK;                                                                     
    }

    resOfScan = sscanf (src, "[%d+%2s]%n", &shift, reg, &ok);                               
                                                                                                                    
    if (ok >= 6) {                                                                          
        src = SkipReadWord (src);    //!TODO copypaste                                                       
        ASSERT_CORRECT (src);                                                               
                                                                                            
        DetectReg (reg);                                                                    
                                                                                            
        FillBitField (field, cmdNumber, typeOfCmd, MEMREGIMM);                             
                                                                                            
        machineCode[(*sizeOfCodeArr)++] = cmdNumber;                                        
        machineCode[(*sizeOfCodeArr)++] = typeOfReg;                                        
        PutInt (shift, machineCode, sizeOfCodeArr);                                        
                                                                                           
        return OK;                                                                         
    }

    resOfScan = sscanf (src, "[%s]%n", reg, &ok);                                          
                                                                                                                    
    if (ok == 4) {                                                                          
        src = SkipReadWord (src);                                                           
        ASSERT_CORRECT (src);                                                               
                                                                                            
        DetectReg (reg);                                                                  
                                                                                        
        FillBitField (field, cmdNumber, typeOfCmd, MEMREG);                                
                                                                                            
        machineCode[(*sizeOfCodeArr)++] = cmdNumber;                                           
        machineCode[(*sizeOfCodeArr)++] = typeOfReg;                                           
                                                                                            
        return OK;                                                                           
    }

    resOfScan = sscanf (src, "%2s", reg);                                                   
    ASSERT_OKAY (resOfScan == 0, return UNKNOWN_REGISTER;)                                                                                       
                                                                                            
    DetectReg (reg);                                                                        
                                                                                            
    src = SkipReadWord (src);                                                             
    ASSERT_CORRECT (src);                                                                   
                                                                                            
    FillBitField (field, cmdNumber, typeOfCmd, REG);                                       
                                                                                            
    machineCode [(*sizeOfCodeArr)++] = cmdNumber;                                              
    machineCode [(*sizeOfCodeArr)++] = typeOfReg;                                              
    return OK;

}

int ArrangeJmpsCall (char *src, const int typeOfCmd, char *machineCode, int *sizeOfCodeArr, Label *lbls, int *labelIp) {
    int dest = -1;
    char commands [10] = {}; //!TODO enum last elem

    src++;                                                                                            
    int resOfScan = sscanf (src, "%s", commands);                                                                            
                                                                                                
    for (int i = 0; i < *labelIp; i++) {                                                        
        if (strncmp (lbls[i].txt, commands, strlen (commands)) == 0) {                                              
            dest = i;                                                                           
            break;                                                                              
        }                                                                                       
    }                                                                                           
    machineCode [(*sizeOfCodeArr)++] = typeOfCmd;                                                  
    if (dest != -1) {                                                                                                                                                                
        PutInt (lbls[dest].ip, machineCode, sizeOfCodeArr);                                    
    }                                                                                           
    else {                                                                                      
        PutInt (-1, machineCode, sizeOfCodeArr);                                               
    }

    return OK;
} 