#include "../lib/stack.h"
#include "../lib/verificator.h"
#include "../lib/dump.h"
#include "../lib/commands.h"

#include "service_functions.h"

int main (const int argc, const char **argv) {   
    CPU softCPU = {};

    softCPU.st = (Stack *)calloc (1, sizeof (Stack));
    softCPU.call = (Stack *)calloc (1, sizeof (Stack));

    InitStack (softCPU.st);
    InitStack (softCPU.call);

    FILE *code = fopen (argv [1], "rb");
    ASSERT_OKAY (code == nullptr, {PrintErrors (UNABLETOOPENFILE); return 0;})

    int fileSize = GetFileSize (code);

    softCPU.machineCode = (char *)calloc (fileSize, sizeof (char));
    ASSERT_OKAY (softCPU.machineCode == nullptr, PrintErrors (NULLPTR));

    softCPU.RAM = (double *)calloc (RAMVOLUME, sizeof (double));
    ASSERT_OKAY (softCPU.RAM == nullptr, PrintErrors (NULLPTR));

    int numberOfReadSym = fread (softCPU.machineCode, sizeof (char), fileSize, code);
    ASSERT_OKAY (numberOfReadSym == 0, PrintErrors (UNABLETOREADFROMFILE));

    RunProccessor (&softCPU);

    DestructStack (softCPU.st);
    DestructStack (softCPU.call);

    free (softCPU.st);
    free (softCPU.call);
    free (softCPU.machineCode);
    free (softCPU.RAM);
    fclose (code);

    return 0;
}