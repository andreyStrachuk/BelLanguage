#include "RecDescent.h"

int main (const int argc, const char *argv []) {
    ASSERT_OK (argc <= 1, PrintErrors (NULLPTR); return 0);
    ASSERT_OK (argv == nullptr, PrintErrors (NULLPTR); return 0);

    Token token = {};

    GetTokens (argv [1], &token);

    MemoryController mem = {};
    MemControllerCtor (&token, &mem);

    int index = 0;

    TreeNode *topNode = GetCode (&token, &index, &mem);

    FILE *dump = fopen ("DumpTree.txt", "w");

    DumpTree (topNode, dump);

    FILE *treetxt = fopen ("tree.txt", "w");

    PrintTreeToFile (topNode, treetxt);

    TreeDestructor (&token, &mem);

    fclose (treetxt);
    
    return 0;
}




int GetTypeOfNode (char *src) {
    assert (src);

    #define KEY_WORD(name, length, c_analogy_name, type, definition, number)        if (STR_EQ (name, src)) { return type; }

    #include "KeyWords.h"

    #undef KEY_WORD

    return FALSE;
}

void GetTokens (const char *fileName, Token *tokens) {
    ASSERT_OK (fileName == nullptr, PrintErrors (NULLPTR); return);
    ASSERT_OK (tokens == nullptr, PrintErrors (NULLPTR); return);

    char *src = GetBuffer (fileName);

    tokens->topNode = (TreeNode *)calloc (strlen (src) + 1, sizeof (TreeNode));


    tokens->numberOfNodes = 0;

    SeparateText (tokens, src);

    free (src);
}

void SeparateText (Token *token, char *src) {
    assert (token);
    assert (src);

    while (*src != '\0') {
        SkipSpaces (src);

        double val = 0;
        int n = 0;
        char dest[30] = {};

        if (*src == '+' || *src == '-') {
	    char tmp[2] = {};
            tmp[0] = *src;
            tmp[1] = '\0';

            src++;

            ALLOC_DATA_FOR_STR ((token->topNode + token->numberOfNodes), tmp, OPERATION);
            token->numberOfNodes++;

            continue;
        }

        sscanf (src, "%lg%n", &val, &n);
        if (n != 0) {
            src += n;

            PushNumberToNode (token->topNode + token->numberOfNodes, val);
            token->numberOfNodes++;

            continue;
        }

        int count = GetWordFromString (dest, src);
        src += count;
 
        int type = GetTypeOfNode (dest);
        if (type == FALSE) {
            SkipSpaces (src);

            if (*src == '(') {
                ALLOC_DATA_FOR_STR ((token->topNode + token->numberOfNodes), dest, FUNCTION);
            }
            else {
                ALLOC_DATA_FOR_STR ((token->topNode + token->numberOfNodes), dest,VARIABLE);
            }

            token->numberOfNodes++;

            continue;
        }

        ALLOC_DATA_FOR_STR ((token->topNode + token->numberOfNodes), dest, type);

        token->numberOfNodes++;
    }
}

void TreeDestructor (Token *token, MemoryController *mem) {
    assert (mem);
    assert (token);

    for (int i = 0; i < token->numberOfNodes; i++) {
        free ((token->topNode + i)->data);
    }

    free (token->topNode);

    for (int i = 0; i < mem->size; i++) {
        free (mem->adr[i]->data);
        free (mem->adr[i]);
    }

    free (mem->adr);
}

void MemControllerCtor (Token *token, MemoryController *mem) {
    assert (mem);

    mem->adr = (TreeNode **)calloc (token->numberOfNodes, sizeof (TreeNode *));
    mem->size = 0;
}

#undef ASSERT_OK
