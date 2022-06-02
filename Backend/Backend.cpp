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
    int res = VarsTableConstruct (&var, varsNum);
    if (res != OK) {
        PrintErrors (res);

        return res;
    }

    ASTtoAsm (node, &var);

    VarsTableDestruct (&var);

    return 0;
}
