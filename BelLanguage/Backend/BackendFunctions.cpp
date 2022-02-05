#include "BackendFunc.h"

double POISON = 1488228328;

static int ifCounter = 0;
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

                table->vars[table->size].addr = table->free;
                table->vars[table->size].name = (char *)node->left->data;
                table->size++;
                table->free++;

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

    fprintf (file, "call : main_foo ;\n");
    
    while (node->type == STATEMENT && node->left != nullptr) node = node->left;

    node = node->right;

    if (STR_EQ ("define", (char *)node->data)) {
        fprintf (file, ": %s\n", (char *)node->left->left->data);

        AssembleStatement (node->right, table, file);
    }

    fprintf (file, "ret ;\n");
    fclose (file);

    return OK;
}

void AssembleStatement (TreeNode *node, VarsTable *table, FILE *file) {
    assert (node);
    assert (table);

    if (node->left != nullptr)
        AssembleStatement (node->left, table, file);

    node = node->right;

    if (STR_EQ ("=", (char *)node->data)) {
        double val = GetTreeVal (node->right->right, table);
        int index = GetVarIndex (node->left, table);

        table->vars[index].value = val;

        fprintf (file, "push %lg ;\n", val);
        fprintf (file, "pop [%d] ;\n\n", table->vars[index].addr);
    }
    if (STR_EQ ("kali", (char *)node->data)) {
        node = node->left->right;

        double val1 = GetTreeVal (node->left, table);
        double val2 = GetTreeVal (node->right, table);

        fprintf (file, "push %lg\npush %lg\n", val1, val2);
        FPrintJump (node, file);
        fprintf (file, " : end_if%d\n", ifCounter);

        node = node->parent->parent->right;

        AssembleStatement (node, table, file);

        fprintf (file, ": end_if%d\n", ifCounter++);
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

double GetTreeVal (TreeNode *node, VarsTable *table) {
    assert (node);
    assert (table);

    if (node->type == NUMBER) {
        return *(double *)node->data;
    }

    if (node->type == VARIABLE) {
        double valvar = GetVarVal (node, table);

        return valvar;
    }

    double val1 = GetTreeVal (node->left, table);

    double val2 = GetTreeVal (node->right, table);

    char symbol = *(char *)node->data;

    switch (symbol) {
        case '+': {
            val1 += val2;

            break;
        }

        case '-': {
            val1 -= val2;

            break;
        }

        case '*': {
            val1 *= val2;

            break;
        }

        case '/': {
            val1 /= val2;

            break;
        }

        default: {
            return val1;
        }
    }

    return val1;
}

int GetResOfCondition (TreeNode *node, VarsTable *table, const double val1, const double val2) {
    assert (node);
    assert (table);

    char symbol = *(char *)node->data;

    if (STR_EQ ("==", (char *)node->data)) {
        return DoubleComp (val1, val2);
    }

    switch (symbol) {
        case '>': {
            return val1 > val2;
        }
        case '<': {
            return val1 < val2;
        }
        default: {
            PrintErrors (INCORRECT_INPUT);
        }
    }

    return INCORRECT_INPUT;
}

void FPrintJump (TreeNode *node, FILE *file) {
    assert (node);

    if (STR_EQ ("==", (char *)node->data)) {
        fprintf (file, "je");

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