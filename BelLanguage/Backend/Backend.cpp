#include "BackendFunc.h"

int main (const int argc, const char *argv []) {
    if (argc < 2) {
        PrintErrors (UNABLETOOPENFILE);

        return 0;
    }

    char *src = GetBuffer (argv [1]);
    
    TreeNode *node = nullptr;
    GetTree (src, &node);

    free (src);

    int varsNum = 0;
    GetApproxNumberOfVars (node, &varsNum);

    VarsTable var = {};
    var.free = 0;
    var.vars = (Variable *)calloc (varsNum, sizeof (Variable));

    ASTtoAsm (node, &var);

    // FILE *file = fopen ("DumpTree.txt", "w");

    // DumpTree (node, file);

    // DestructTree (node);
    // free (node);

    return 0;
}
