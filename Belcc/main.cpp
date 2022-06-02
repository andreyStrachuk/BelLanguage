#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main (const int argc, const char *argv[]) {
    if (argc < 2) {
        printf ("Unable to open file!\n");
        return -1;
    }

    char command[20] = "./front ";
    strcat (command, argv[1]);
    
    system (command);

    system ("./back tree.txt");

    system ("lib/SoftCPU/./asm lang.asm");

    system ("lib/SoftCPU/./cpu code.bin");

    return 0;
}