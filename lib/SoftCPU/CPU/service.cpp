#include "service_functions.h"

char N_PUSH [] = "push";
char N_POP [] = "pop";

#define DEF_REG_(reg, code)                 \
            case R_##reg: {                 \
                return softCPU->reg;        \
            }                               \


double GetRegValue (const int reg, CPU *softCPU) {
    switch (reg)
    {
        #include "../Assembler/regs_def.h"
        
        default:
            return UNKNOWN_REGISTER;
    }
}

#undef DEF_REG_

#define DEF_REG_(reg, code)                 \
        case R_##reg: {                     \
            softCPU->reg = val;             \
            break;                          \
        }                                   \


int WriteToRegister (const int regType, CPU *softCPU, double val) {
    assert (softCPU);

    switch (regType) {
        #include "../Assembler/regs_def.h"

        #undef DEF_REG_

        default : 
            return UNKNOWN_REGISTER;
    }

    return OK;

}

int CheckSignature (char *src) {
    assert (src);

    int res = strncmp (src, SIGN, 2);
    ASSERT_OKAY (res != 0, {return WRONGFILETYPE;} )

    return OK;
}

int CheckIfImm (const int type) {
    if (type & 0x20 && !(type & 0x40) && !(type & 0x80)) {
        return IMM;
    }
    
    return 0;
}

int CheckIfReg (const int type) {
    if (type & 0x40 && !(type & 0x80) && !(type & 0x20)) { 
        return REG;
    }

    return 0;
}

int CheckIfMem (const int type) {
    if (type & 0x80 && !(type & 0x40) && !(type & 0x20)) {
        return MEM;
    }
    else if (type & 0x80 && type & 0x40 && !(type & 0x20)) {
        return MEMREG;
    }
    else if (type & 0x80 && type & 0x40 && type & 0x20) {
        return MEMREGIMM;
    }

    return 0;
}

#define DEF_CMD_(name, cmdNumb, argNumber, code, function)                                              \
                    case CMD_##name: {                                                                  \
                        char str [] = #name;                                                            \
                        switch (argNumber) {                                                            \
                            case 0:                                                                     \
                                code                                                                    \
                                continue;                                                               \
                            case 1: {                                                                   \
                                res = DetectPushnPop (str, softCPU, type);                              \
                                ASSERT_OKAY (res != OK, return res;)                                    \
                                continue;                                                               \
                                break;                                                                  \
                            }                                                                           \
                            case 2: {                                                                   \
                                code                                                                    \
                                break;                                                                  \
                            }                                                                           \
                            default : {                                                                 \
                                return UNKNOWN_COMMAND;                                                 \
                            }                                                                           \
                        }                                                                               \
                                                                                                        \
                        softCPU->ip++;                                                                  \
                        break;                                                                          \
                    }                                                                                   \


int RunCPU (CPU *softCPU) {
    assert (softCPU);

    int res = CheckSignature (softCPU->machineCode);
    ASSERT_OKAY (res != OK, return WRONGFILETYPE;)
    softCPU->machineCode += 2;

    double topVal = 0;
    double prevTopVal = 0;

    int index = 0;
    int shift = 0;
    double val = 0;

    while (1) {
        int cmd = *(softCPU->machineCode + softCPU->ip) & 0x1F;
        int type = *(unsigned char *)(softCPU->machineCode + softCPU->ip) & 0xE0;

        softCPU->ip++;

        int res = 0;

        switch (cmd) {
            #include "../Assembler/commands_def.h"

        }   

        #undef DEF_CMD_
    }

}

int DetectPushnPop (char *srcName, CPU *softCPU, const int type) { //!TODO separate on static functions
    int res = CheckIfImm (type);
    double val = 0;
    int shift = 0;
    int index = 0;

    res = CheckIfImm (type);                                                
    if (res == IMM) {                                                       
        if (STR_EQ (srcName, N_PUSH)) {                                       
            val = *(double *)(softCPU->machineCode + softCPU->ip);          
                                                                            
            PUSHSTACK();                                                            
                                                                            
            softCPU->ip += sizeof (double);                                 
            return OK;                                                       
        }                                                                   
        if (STR_EQ (srcName, N_POP)) {                                        
            POPSTACK();                                         
        }                                                                   
        return OK;                                                           
    }                                                                       
                                                                            
    res = CheckIfReg (type);                                                
    if (res == REG) {                                                       
        int reg = *(char *)(softCPU->machineCode + softCPU->ip);            
        softCPU->ip++;                                                      
        if (STR_EQ (srcName, N_PUSH)) {                                       
                                                                            
            val = GetRegValue (reg, softCPU);                               
                                                                            
            PUSHSTACK();                                   
        }                                                                   
        if (STR_EQ (srcName, N_POP)) {                                       
                                                                            
            val = POPSTACK();                                   
                                                                            
            res = WriteToRegister (reg, softCPU, val);                      
                                                                            
            ASSERT_OKAY (res == UNKNOWN_REGISTER, return UNKNOWN_REGISTER; )
        }                                                                   
        return OK;                                                           
    }                                                                       
                                                                            
    res = CheckIfMem (type);                                                
    if (res == MEM) {                                                       
        shift = *(u_int16_t *)(softCPU->machineCode + softCPU->ip);         
                                                                            
        softCPU->ip += 2;                                                   
                                                                            
        ASSERT_OKAY (index > RAMVOLUME, return WRONG_ADDRESS;)

        if (STR_EQ (srcName, N_PUSH)) {                                       
                                                                            
            GETRAMVAL (index);                                     
                                                                            
            PUSHSTACK();                                   
        }                                                                   
        if (STR_EQ (srcName, N_POP)) {                                        
                                                                            
            val = POPSTACK();                                   
                                                                            
            WRITETORAM (shift);                                    
        }                                                                   
        return OK;                                                          
    }                                                                       
    if (res == MEMREG) {                                                    
        int typeOfReg = *(char *)(softCPU->machineCode + softCPU->ip);      
                                                                            
        softCPU->ip++;                                                      
                                                                            
        index = GetRegValue (typeOfReg, softCPU);                           
                                                                            
        ASSERT_OKAY (index > RAMVOLUME, return WRONG_ADDRESS; )                                                              
                                                                           
        if (STR_EQ (srcName, N_PUSH)) {            
            GETRAMVAL (index);

            PUSHSTACK();                                   
        }                                                                   
        if (STR_EQ (srcName, N_POP)) {                                        
            val = POPSTACK();                                   
                                                                      
            WRITETORAM (index);                                     
        }                                                                   
        return OK;                                                           
    }                                                                       
    if (res == MEMREGIMM) {                                                 
        int typeOfReg = *(softCPU->machineCode + softCPU->ip);         
                                                                            
        softCPU->ip++;                                                      
                                                                            
        double regValue = GetRegValue (typeOfReg, softCPU);                 
                                                                            
        shift = *(u_int16_t *)(softCPU->machineCode + softCPU->ip);         
        softCPU->ip += sizeof (u_int16_t);                                                   
                                                                            
        index = (int)regValue + shift;                                      
                                                                            
        ASSERT_OKAY (index > RAMVOLUME, return WRONG_ADDRESS; )                                                                  
                                                                            
        if (STR_EQ (srcName, N_PUSH)) {                                  
            GETRAMVAL (index);                                      
                                                                            
            PUSHSTACK();                                   
        }                                                                   
        if (STR_EQ (srcName, N_POP)) {                             
            val = POPSTACK();                                   
                                                                            
            WRITETORAM (index);                                     
        }                                                                   
        return OK;                                                           
    }                                                                       

    return OK;                                                                                                                                                                                                         
}

