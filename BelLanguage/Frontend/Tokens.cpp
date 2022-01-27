#include "RecDescent.h"

#define ASSERT_OK(smth, doSmth)         do { if (smth) { doSmth;}} while (0)

#define SkipSpaces(src)                         src = SkipSpaceSymbols (src);   \
                                                if (*src == '\0') {             \
                                                    break;                      \
                                                }

void GetTokens (const char *fileName, Token *tokens) {
    ASSERT_OK (fileName == nullptr, PrintErrors (NULLPTR); return);
    ASSERT_OK (tokens == nullptr, PrintErrors (NULLPTR); return);

    char *src = nullptr;
    GetBuffer (&src, fileName);

    printf ("%s", src);

    tokens->topNode = (TreeNode *)calloc (strlen (src) + 1, sizeof (TreeNode));
    tokens->numberOfNodes = 0;

    while (*src != '\0') {
        SkipSpaces (src);

        double val = 0;
        int n = 0;
        char dest[30] = {};

        sscanf (src, "%lg%n", &val, &n);
        if (n != 0) {
            src += n;

            PushNumberToNode (tokens->topNode + tokens->numberOfNodes, val);
            tokens->numberOfNodes++;

            printf ("%lg\n", val);

            continue;
        }

        int count = GetWordFromString (dest, src);
        src += count;

        int type = GetTypeOfNode (dest);
        if (type == FALSE) {
            SkipSpaces (src);

            if (*src == '(') {
                ALLOC_DATA_FOR_STR ((tokens->topNode + tokens->numberOfNodes), dest, FUNCTION);
            }
            else {
                ALLOC_DATA_FOR_STR ((tokens->topNode + tokens->numberOfNodes), dest,VARIABLE);
            }

            tokens->numberOfNodes++;

            continue;
        }

        ALLOC_DATA_FOR_STR ((tokens->topNode + tokens->numberOfNodes), dest, type);

        tokens->numberOfNodes++;
    }
}

int main (const int argc, const char *argv []) {
    ASSERT_OK (argc <= 1, PrintErrors (NULLPTR); return 0);
    ASSERT_OK (argv == nullptr, PrintErrors (NULLPTR); return 0);

    Token token = {};

    GetTokens (argv [1], &token);

    printf ("number of tokens - %d\n", token.numberOfNodes);

    printf ("type of token - %d\n token's name - %s\n", token.topNode->type, (char *)token.topNode->data);

    int index = 0;

    TreeNode *topNode = GetFuncDef (&token, &index);

    FILE *dump = fopen ("DumpTree.txt", "w");

    printf ("pointer - %p\nname - %s\n", (void *)topNode, (char *)topNode->data);

    DumpTree (topNode, dump);
    
    return 0;
}

#undef ASSERT_OK

int GetTypeOfNode (char *src) {
    assert (src);

    #define KEY_WORD(name, length, c_analogy_name, type, definition, number)        if (STR_EQ (name, src)) { return type; }

    #include "KeyWords.h"

    #undef KEY_WORD

    return FALSE;
}