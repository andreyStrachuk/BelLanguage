DEF_CMD_(hlt, 0x00, 0, 
{
    printf("Program has finished!\n");
    softCPU->machineCode -= 2;
    return OK;
}, ;)

DEF_CMD_(push, 0x01, 1,
{
    PUSHSTACK();
}, 
{
    ARRANGEPUSHANDPOPCMDS();                                                                     
} )

DEF_CMD_(pop, 0x02, 1, 
{
    topVal = POPSTACK();
}, 
{
    ARRANGEPUSHANDPOPCMDS();
} )

DEF_CMD_(add, 0x03, 0, 
{
    topVal = POPSTACK();
    prevTopVal = POPSTACK();

    val = topVal + prevTopVal;

    PUSHSTACK();
}, ; )

DEF_CMD_(mul, 0x04, 0, 
{
    topVal = POPSTACK();
    prevTopVal = POPSTACK();

    val = topVal * prevTopVal;

    PUSHSTACK();
}, ; )

DEF_CMD_(sub, 0x05, 0,
{
    topVal = POPSTACK();
    prevTopVal = POPSTACK();

    val = topVal - prevTopVal;

    PUSHSTACK();
}, ; )

DEF_CMD_(out, 0x06, 0,
{
    topVal = POPSTACK();

    printf ("Top stack value: %lg\n", topVal);

}, ; )

DEF_CMD_(in, 0x07, 0,
{
    printf ("Enter a real number: ");
    scanf ("%lg", &val);

    PUSHSTACK();
    continue;
}, ; )

DEF_CMD_(sqrt, 0x08, 0, 
{
    val = POPSTACK();

    val = sqrt (val);

    PUSHSTACK();
}, ; )

DEF_CMD_(div, 0x09, 0, 
{
    topVal = POPSTACK();
    prevTopVal = POPSTACK();

    val = topVal / prevTopVal;

    PUSHSTACK();
}, ; )

DEF_CMD_(cos, 0x0A, 0, 
{
    topVal = POPSTACK();

    val = cos (val);

    PUSHSTACK();
}, ; )

DEF_CMD_(sin, 0x0B, 0, 
{
    topVal = POPSTACK();

    val = cos (val);

    PUSHSTACK();
}, ; )

DEF_CMD_(jmp, 0x0C, 2,
{
    int index = softCPU->ip;
    softCPU->ip = *(u_int16_t *)(softCPU->machineCode + index);
    continue;
}, 
{
    ARRANGEJMPS();
} )

DEF_CMD_(je, 0x0D, 2,
{
    topVal = POPSTACK();
    prevTopVal = POPSTACK();

    if (fabs(topVal - prevTopVal) < 1e-6) {
        int index = softCPU->ip;
        softCPU->ip = *(u_int16_t *)(softCPU->machineCode + index);

        int retVal = index + 2;

        PUSHCALLSTACK();
        continue;
    }
    SKIPJMPVAL();
    continue;
}, 
{
    ARRANGEJMPS();
} )

DEF_CMD_(jne, 0x0E, 2,
{
    topVal = POPSTACK();
    prevTopVal = POPSTACK();

    if (fabs(topVal - prevTopVal) > 1e-6) {
        int index = softCPU->ip;
        softCPU->ip = *(u_int16_t *)(softCPU->machineCode + index);

        int retVal = index + 2;

        PUSHCALLSTACK();
        continue;
    }
    SKIPJMPVAL();
    continue;
},
{
    ARRANGEJMPS();
} )

DEF_CMD_(ja, 0x0F, 2,
{
    topVal = POPSTACK();
    prevTopVal = POPSTACK();

    if (topVal > prevTopVal) {
        int index = softCPU->ip;
        softCPU->ip = *(u_int16_t *)(softCPU->machineCode + index);
        continue;
    }

    SKIPJMPVAL();
    continue;
},
{
    ARRANGEJMPS();
} )

DEF_CMD_(dec, 0x10, 0,
{
    topVal = POPSTACK();

    PushStack (softCPU->st, (int)topVal - 1);
}, ; )

DEF_CMD_(ret, 0x11, 0,
{
    int index = (int)POPCALLSTACK();

    softCPU->ip = index;
    continue;
}, 
{
    ARRANGEJMPS(); 
} )

DEF_CMD_(call, 0x12, 2,
{
    int index = softCPU->ip;
    softCPU->ip = *(u_int16_t *)(softCPU->machineCode + index);

    int retVal = index + 2;

    PUSHCALLSTACK();
    continue;
}, 
{
    ARRANGEJMPS();
} )

DEF_CMD_(inc, 0x13, 0,
{
    topVal = POPSTACK();

    val = (int)topVal + 1;

    PUSHSTACK();
}, ; )

DEF_CMD_(jb, 0x14, 2,
{
    topVal = POPSTACK();
    prevTopVal = POPSTACK();

    if (topVal < prevTopVal) {
        int index = softCPU->ip;
        softCPU->ip = *(u_int16_t *)(softCPU->machineCode + index);

        int retVal = index + 2;

        PUSHCALLSTACK();
        continue;
    }

    SKIPJMPVAL();
    continue;
}, 
{
    ARRANGEJMPS();
} )

DEF_CMD_(jae, 0x15, 2,
{
    topVal = POPSTACK();
    prevTopVal = POPSTACK();

    if (topVal >= prevTopVal) {
        int index = softCPU->ip;
        softCPU->ip = *(u_int16_t *)(softCPU->machineCode + index);
        int retVal = index + 2;

        PUSHCALLSTACK();
        continue;
    }

    SKIPJMPVAL();
    continue;
},
{
    ARRANGEJMPS();
} )