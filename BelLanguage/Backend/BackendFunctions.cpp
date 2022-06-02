#include "BackendFunc.h"

double POISON = 1488228328;

// return register --- ax
static const char returnReg[] = "ax";

static int ifCounter = 0;
static int whileCounter = 0;
static int globalFree = 0;
// static int funcCounter = 0;

int GetTree (char *exp, TreeNode **topNode) {
    assert (exp);
    assert (topNode);

    if (*topNode == nullptr) {
        *topNode = (TreeNode *)calloc(1, sizeof (TreeNode));
        exp++;
    }

    TreeNode *node = *topNode;

    while (1) {
        SkipSpaces (exp);

        if (*exp == '(') {
            if (node->type == 0) {
                node->left = (TreeNode *)calloc (1, sizeof (TreeNode));
                node->left->parent = node;

                node = node->left;
            }
            else {
                node->right = (TreeNode *)calloc (1, sizeof (TreeNode));
                node->right->parent = node;

                node = node->right;
            }

            exp++;

            continue;
        }

        if (*exp == ')') {
            exp++;

            node = node->parent;

            continue;
        }

        int n = 0;
        int type = 0;

        sscanf (exp, "%d%n", &type, &n);
        if (n == 0) {
            PrintErrors (INCORRECT_INPUT);
            break;
        }

        exp += n;
        SkipSpaces (exp);
        n = 0;

        if (type == NUMBER) {
            double val = 0;
            sscanf (exp, "%lg%n", &val, &n);

            exp += n;

            PushNumberToNode (node, val);
            continue;
        }

        char arg[50] = {};
        GetWord (arg, &exp);

        ALLOC_DATA_FOR_STR (node, arg, type);

        if (*exp == '\0') break;
    }

    return OK;
}

void GetApproxNumberOfVars (TreeNode *topNode, int *varsNum) {
    assert (topNode);

    if (topNode->type == VARIABLE) {
        (*varsNum)++;
    }

    if (topNode->right == nullptr) {
        return;
    }

    if (topNode->left != nullptr) {
        GetApproxNumberOfVars (topNode->left, varsNum);
    }

    GetApproxNumberOfVars (topNode->right, varsNum);
}

int FillVarsTable (TreeNode *node, VarsTable *table) {
    assert (node);
    assert (table);

    int res = 0;

    if (node->right != nullptr && node->right->type == VARIABLE) {
        res = CheckIfVarInTable ((char *)node->right->data, table);

        if (res == FALSE) {
            return INCORRECT_INPUT;
        }
    }

    if (node->left != nullptr) {
        if (node->type != NUMBER && STR_EQ ("=", (char *)node->data) && node->left->type == VARIABLE) {
            res = CheckIfVarInTable ((char *)node->left->data, table);

            if (res == FALSE) {

                table->vars[table->size].shift = table->free;
                table->vars[table->size].name = (char *)node->left->data;
                table->size++;
                table->free++;
                globalFree = table->free;
            }
        }
    }

    if (node->right == nullptr) {
        return OK;
    }

    if (node->left != nullptr) {
        FillVarsTable (node->left, table);
    }

    FillVarsTable (node->right, table);

    return OK;
}

int CheckIfVarInTable (const char *str, VarsTable *table) {
    assert (str);
    assert (table);

    for (int i = 0; i < table->size; i++) {
        if (STR_EQ (str, table->vars[i].name)) {
            return TRUE;
        }
    }

    return FALSE;
}

int ASTtoAsm (TreeNode *node, VarsTable *table) {
    assert (node);
    assert (table);

    FILE *file = fopen ("lang.asm", "w");

    fprintf (file, "call : main ;\n");
    fprintf (file, "hlt\n");
    
    AssembleFunctions (node, file);

    fclose (file);

    return OK;
}

void AssembleStatement (TreeNode *node, VarsTable *table, FILE *file, const int widthOfStackFrame) {
    assert (node);
    assert (table);

    if (node->left != nullptr)
        AssembleStatement (node->left, table, file, widthOfStackFrame);

    node = node->right;

    if (STR_EQ ("=", (char *)node->data)) {
        if (STR_EQ ("call", (char *)node->right->right->data)) {
            AssembleStatement (node->right, table, file, widthOfStackFrame);

            fprintf (file, "push ax\n");
        }
        else {
            GetTreeVal (node->right->right, table, file);
        }

        int addr = GetVarAddr(node->left, table);

        fprintf (file, "pop [%d+bx] ;\n\n", addr);
    }
    if (STR_EQ ("kali", (char *)node->data)) {
        node = node->left->right;

        GetTreeVal (node->left, table, file);
        GetTreeVal (node->right, table, file);

        FPrintJump (node, file);
        fprintf (file, " : end_if%d\n", ifCounter);

        printf ("IN JUMP!!!!!!!!!\nifCounter - %d\n", ifCounter);

        node = node->parent->parent->right;

        AssembleStatement (node, table, file, widthOfStackFrame);

        fprintf (file, ": end_if%d\n", ifCounter++);
    }
    if (STR_EQ ("pakul", (char *)node->data)) {
        node = node->left->right;

        fprintf (file, ": while_%d\n", whileCounter);

        GetTreeVal (node->left, table, file);
        GetTreeVal (node->right, table, file);

        FPrintJump (node, file);
        fprintf (file, " : while_%d_end\n", whileCounter);

        node = node->parent->parent->right;

        AssembleStatement (node, table, file, widthOfStackFrame);

        fprintf (file, "jmp : while_%d\n", whileCounter);
        fprintf (file, ": while_%d_end\n", whileCounter++);
    }

    if (STR_EQ ("call", (char *)node->data)) {
        if (STR_EQ ("nadrukavac", (char *)node->left->data)) {
            AssemblePrintFunction (node->right, table, file);
        }
        else if (STR_EQ("atrymac", (char *)node->left->data)) {
            AssembleScanFunction (node->right, table, file);
        }
        else if (STR_EQ("sqrt", (char *)node->left->data)) {
            AssembleSqrtFunction (node->right, table, file);
        }
        else {
            LoadParamsToRAM (node->right, table, file);

            fprintf (file, "call : %s\n", (char *)node->left->data);
        }
    }

    if (STR_EQ ("vyarnut", (char *)node->data)) { 
        if (node->right->type == VARIABLE) {
            int addr = GetVarAddr (node->right, table);

            fprintf (file, "push [%d+bx]\n", addr);
        }
        else if (node->right->type == NUMBER) {
            double val = *(double *)node->right->data;

            fprintf (file, "push %lg\n", val);
        }
        else {
            PrintErrors (INCORRECT_INPUT);
            return;
        }

        fprintf (file, "pop %s\n", returnReg);

        fprintf (file,  "push %d\n"
                        "push bx\n"
                        "sub\n"
                        "pop bx\n", widthOfStackFrame);

        fprintf (file, "ret\n");
    }

    return;
}

double GetVarVal (TreeNode *node, VarsTable *table) {
    assert (node);
    assert (table);

    for (int i = 0; i < table->size; i++) {
        if (STR_EQ ((char *)node->data, table->vars[i].name)) {
            return table->vars[i].value;
        }
    }

    return NULLADDR;
}

int GetVarIndex (TreeNode *node, VarsTable *table) {
    assert (node);
    assert (table);

    for (int i = 0; i < table->size; i++) {
        if (STR_EQ ((char *)node->data, table->vars[i].name)) {
            return i;
        }
    }

    return NULLADDR;
}

void GetTreeVal (TreeNode *node, VarsTable *table, FILE *file) {
    assert (node);
    assert (table);

    if (node->type == NUMBER) {
        fprintf (file, "push %lg\n", *(double *)node->data);

        return;
    }

    if (node->type == VARIABLE) {
        int addr = GetVarAddr (node, table);

        fprintf (file, "push [%d+bx]\n", addr);

        return;
    }

    assert (node->right && "1 FUCK YOU MAN!\n");
    GetTreeVal (node->right, table, file);

    assert (node->left && "2 FUCK YOU MAN!\n");
    GetTreeVal (node->left, table, file);

    #define KEY_WORD(name, length, c_analogy_name, type, definition, number)        if (STR_EQ (name, (char *)node->data)) { fprintf (file, "%s\n", definition); return; }

    #include "../Frontend/KeyWords.h"

    #undef KEY_WORD

    return;
}

void FPrintJump (TreeNode *node, FILE *file) {
    assert (node);

    if (STR_EQ ("==", (char *)node->data)) {
        fprintf (file, "jne");

        return;
    }

    switch (*(char *)node->data) {
        case '>': {
            fprintf (file, "ja");

            break;
        }

        case '<': {
            fprintf (file, "jb");

            break;
        }

        default: {
            fprintf (file, "Unknown jump!!!\n");
        }
    }
}

int GetVarAddr (TreeNode *node, VarsTable *table) {
    assert (node);
    assert (table);

    for (int i = 0; i < table->size; i++) {
        if (STR_EQ ((char *)node->data, table->vars[i].name)) {
            return table->vars[i].shift;
        }
    }

    return NULLADDR;
}

int AssemblePrintFunction (TreeNode *node, VarsTable *table, FILE *file) {
    assert (node);
    assert (table);
    assert (file);

    if (node->right == nullptr) {
        PrintErrors (INCORRECT_INPUT);
        
        return INCORRECT_INPUT;
    }

    if (node->left != nullptr) {
        AssemblePrintFunction (node->left, table, file);
    }

    if (node->right->type == VARIABLE) {
        int addr = GetVarAddr (node->right, table);
        ASSERT_OK (addr == NULLADDR, PrintErrors (INCORRECT_INPUT); return INCORRECT_INPUT);

        fprintf (file, "push [%d+bx]\n", addr);
        fprintf (file, "out\n");
        fprintf (file, "push [%d+bx]\n", addr);
    }
    if (node->right->type == NUMBER) {
        double val = *(double *)node->right->data;

        fprintf (file, "push %lg\n", val);
        fprintf (file, "out\n");
    }

    return OK;
}

int AssembleScanFunction (TreeNode *node, VarsTable *table, FILE *file) {
    assert (node);
    assert (table);
    assert (file);

    if (node->right == nullptr) {
        PrintErrors (INCORRECT_INPUT);
        
        return INCORRECT_INPUT;
    }

    if (node->left != nullptr) {
        AssembleScanFunction (node->left, table, file);
    }

    int addr = GetVarAddr (node->right, table);
    ASSERT_OK (addr == NULLADDR, PrintErrors (INCORRECT_INPUT); return INCORRECT_INPUT);

    fprintf (file, "in\n");
    fprintf (file, "pop [%d+bx]\n", addr);

    return OK;
}

int AssembleSqrtFunction (TreeNode *node, VarsTable *table, FILE *file) {
    assert (node);
    assert (table);
    assert (file);

    if (node->right == nullptr) {
        PrintErrors (INCORRECT_INPUT);
        
        return INCORRECT_INPUT;
    }

    int addr = GetVarAddr (node->right, table);
    ASSERT_OK (addr == NULLADDR, PrintErrors (INCORRECT_INPUT); return INCORRECT_INPUT);

    fprintf (file, "push [%d+bx]\n", addr);
    fprintf (file, "sqrt\n");
    fprintf (file, "pop %s\n", returnReg);

    return OK;
}

void AssembleFunctions (TreeNode *node, FILE *file) {
    assert (node);
    assert (file);

    if (node->left != nullptr) {
        AssembleFunctions (node->left, file);
    }

    node = node->right;
    
    fprintf (file, ": %s\n", (char *)node->left->left->data);

    int numberOfParams = GetNumberOfParams (node->left->right);
    int numberOfVars = 0;
    GetApproxNumberOfVars (node->right, &numberOfVars);

    printf ("approx number of vars - %d\napprox number of params - %d\n", numberOfVars, numberOfParams);

    printf ("number of params - %d\nnumberof vars - %d\n", numberOfParams, numberOfVars);

    VarsTable tableOfVars {};
    tableOfVars.free = globalFree;
    tableOfVars.startAddr = globalFree;
    tableOfVars.size = 0;
    tableOfVars.vars = (Variable *)calloc (numberOfParams + numberOfVars, sizeof (Variable));

    fprintf (file,  "push %d\n"
                    "push bx\n"
                    "add\n"
                    "pop bx\n", numberOfParams + numberOfVars);

    if (numberOfParams != 0) {
        printf ("I AM FUCKING HERE!!!!!!!!\n");
        FillParametersToTable (node->left->right, &tableOfVars, file);
    }

    FillVarsTable (node->right, &tableOfVars);

    AssembleStatement (node->right, &tableOfVars, file, numberOfParams + numberOfVars);

    fprintf (file,  "push %d\n"
                    "push bx\n"
                    "sub\n"
                    "pop bx\n", numberOfParams + numberOfVars);

    fprintf (file, "ret ;\n");
}

int GetNumberOfParams (TreeNode *node) {
    assert (node);

    static int number = 1;

    if (node->right == nullptr) {
        return 0;
    }

    if (node->left != nullptr) {
        number++;

        GetNumberOfParams (node->left);
    }

    return number;
}

int FillParametersToTable (TreeNode *node, VarsTable *table, FILE *file) {
    assert (node);
    assert (table);

    if (node->left != nullptr) {
        FillParametersToTable (node->left, table, file);
    }

    table->vars[table->size].name = (char *)node->right->data;
    table->vars[table->size].shift = table->free;
    fprintf (file, "pop [%d+bx]\n", table->vars[table->size].shift);
    table->free++;
    table->size++;
    globalFree = table->free;

    return OK;
}

int LoadParamsToRAM (TreeNode *node, VarsTable *table, FILE *file) {
    assert (node);
    assert (table);
    assert (file);

    if (node->right->type == NUMBER) {
        fprintf (file, "push %lg\n", *(double *)node->right->data);
    }
    else if (node->right->type == VARIABLE) {
        int addr = GetVarAddr (node->right, table);

        fprintf (file, "push [%d+bx]\n", addr);
    }
    else {
        PrintErrors (INCORRECT_INPUT);
        
        return INCORRECT_INPUT;
    }

    if (node->left != nullptr) {
        LoadParamsToRAM (node->left, table, file);
    }

    return OK;
}