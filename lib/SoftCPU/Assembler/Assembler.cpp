#include "general_functions.h"

int main (const int argc, const char **argv) {
    FILE *asmProgFile = fopen (argv [1], "rb");
    ASSERT_OKAY (asmProgFile == nullptr, {PrintErrors (UNABLETOOPENFILE); return 0;});

    FILE *code = fopen ("code.bin", "wb"); 
    ASSERT_OKAY (code == nullptr, {PrintErrors (UNABLETOOPENFILE); return 0;});

    int fileSize = GetFileSize (asmProgFile);
    int numOfStrings = NumberOfStrings (asmProgFile);   
    int labelIp = 0; 
    int result = 0;

    char *asmProg = (char *)calloc (fileSize, sizeof (char)); 
    ASSERT_OKAY(asmProg == nullptr, PrintErrors (NULLPTR));

    fileSize = fread (asmProg, sizeof (char), fileSize, asmProgFile);

    Line *cmds = (Line *)calloc (numOfStrings, sizeof (Line));
    ASSERT_OKAY(cmds == nullptr, PrintErrors(NULLPTR));

    Label *lbls = (Label *)calloc (NUMBEROFLABELS, sizeof (Label));
    ASSERT_OKAY(lbls == nullptr, PrintErrors(NULLPTR));

    InitializeArrOfPointers (cmds, asmProg, numOfStrings);

    ReadCmdsAndWrite (code, cmds, numOfStrings, lbls, labelIp);

    MemFree (asmProg, cmds, numOfStrings, lbls, labelIp);

    fclose (code);
    fclose (asmProgFile);
    
    return 0;
}
