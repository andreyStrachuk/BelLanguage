#include "RecDescent.h"

static const double POISON = 1488228;

TreeNode *GetG (Token *token, int *index) {
    assert (token);
    assert (index);

    TreeNode *node = GetStatement (token, index);

    //if (STR_EQ ((char *)token->topNode[*index].data, "\n")) {
        return node;
    //}
    //else {
    //    return 0;
    //}
}

TreeNode *GetE (Token *token, int *index) {
    assert (token);
    assert (index);

    TreeNode *node1 = GetT (token, index);

    TreeNode *nodeOp = nullptr;

    while (*(char *)token->topNode[*index].data == '+' || *(char *)token->topNode[*index].data == '-') {
        nodeOp = &(token->topNode[*index]);

        (*index)++;

        TreeNode *node2 = GetT (token, index);

        nodeOp->left = node1;
        nodeOp->right = node2;

        node1->parent = nodeOp;
        node2->parent = nodeOp;

        node1 = nodeOp;
    }

    return node1;
}

TreeNode *GetT (Token *token, int *index) {
    assert (token);
    assert (index);

    TreeNode *node1 = GetExponent (token, index);

    while (*(char *)token->topNode[*index].data == '*' || *(char *)token->topNode[*index].data == '/') {
        TreeNode *nodeOp = &(token->topNode[*index]);

        (*index)++;

        TreeNode *node2 = GetExponent (token, index);

        nodeOp->left = node1;
        nodeOp->right = node2;

        node1->parent = nodeOp;
        node2->parent = nodeOp;

        node1 = nodeOp;
    }

    return node1;
}

TreeNode *GetP (Token *token, int *index) {
    assert (token);
    assert (index);

    if (*(char *)(token->topNode + *index)->data ==  '(') {
        (*index)++;

        TreeNode *node = GetE (token, index);

        Require (token, ")", index);

        return node;

    }
    else if ((token->topNode + *index)->type == VARIABLE) {
        if (*index + 1 < token->numberOfNodes) 
            return (token->topNode + (*index)++);

        return (token->topNode + *index);
    }
    else if (STR_EQ ("kali", (char *)(token->topNode + *index)->data) || STR_EQ ("pakul", (char *)(token->topNode + *index)->data)) {
        TreeNode *nodeIf = (token->topNode + *index);
        (*index)++;

        Require (token, "(", index);

        TreeNode *nodeCond = GetCondition (token, index);

        Require (token, ")", index);

        Require (token, "begin", index);

        TreeNode *nodeStatement = GetStatement (token, index);

        Require (token, "end", index);

        nodeIf->left = nodeCond;
        nodeIf->right = nodeStatement;
        
        return nodeIf;
    }
    else {
        return GetN (token, index);
    }
}

TreeNode *GetN (Token *token, int *index) {
    assert (token);
    assert (index);

    TreeNode *node = nullptr;

    if ((token->topNode + *index)->type == NUMBER) {
        node = (token->topNode + *index);

        node->right = nullptr;
        node->left = nullptr;
    }

    if (*index + 1 < token->numberOfNodes) 
        (*index)++;

    printf ("%d\n", *index);

    return node;
}

TreeNode *GetAssign (Token *token, int *index) {
    assert (token);
    assert (index);

    TreeNode *node1 = GetE (token, index);

    if (*(char *)token->topNode[*index].data == '=') {
        if (token->topNode[*index - 1].type != VARIABLE) {
            SyntaxError ();
        }

        TreeNode *nodeOp = &(token->topNode[*index]);

        (*index)++;

        TreeNode *id = (TreeNode *)calloc (1, sizeof (TreeNode));
        ALLOC_DATA_FOR_STR (id, "identifier", IDENTIFIER);

        TreeNode *node2 = GetE (token, index);

        id->left = nullptr;
        id->right = node2;
        id->parent = nodeOp;

        node1->parent = nodeOp;
        node2->parent = id;

        nodeOp->left = node1;
        nodeOp->right = id;

        node1 = nodeOp;
    }

    return node1;
}

TreeNode *GetExponent (Token *token, int *index) {
    assert (token);
    assert (index);

    TreeNode *node1 = GetP (token, index);
    

    while (*(char *)token->topNode[*index].data == '^') {
        printf ("In getexp!\n");
        (*index)++;

        TreeNode *node2 = GetP (token, index);

        TreeNode *nodeOp = &(token->topNode[*index - 1]);

        node1->parent = nodeOp;
        node2->parent = nodeOp;

        nodeOp->left = node1;
        nodeOp->right = node2;

        node1 = nodeOp;
    }

    printf ("Dv!!\n");

    return node1;
}

TreeNode *GetFuncDef (Token *token, int *index) {
    assert (token);
    assert (index);

    TreeNode *nodeDef = nullptr;

    if ((token->topNode + *index)->type == FUNCTION) {
        TreeNode *nodeName = (token->topNode + *index);
        (*index)++;

        Require (token, "(", index);

        TreeNode *args = GetArgs (token, index);

        printf ("pointer - %p\n", (void *)args);

        Require (token, ")", index);

        Require (token, "begin", index);
        
        TreeNode *nodeIn = GetStatement (token, index);

        Require (token, "end", index);

        TreeNode *nodeFunc = (TreeNode *)calloc (1, sizeof (TreeNode));
        ALLOC_DATA_FOR_STR (nodeFunc, "function", FUNCTION);

        nodeFunc->left = nodeName;
        if (args != nullptr) nodeFunc->right = args;

        nodeName->parent = nodeFunc;
        if (args != nullptr) args->parent = nodeFunc;

        nodeDef = (TreeNode *)calloc (1, sizeof (TreeNode));
        ALLOC_DATA_FOR_STR (nodeDef, "define", OPERATION);

        nodeDef->left = nodeFunc;
        nodeDef->right = nodeIn;

        nodeIn->parent = nodeDef;
        nodeIn->parent = nodeDef;
    }

    return nodeDef;
}

TreeNode *GetArgs (Token *token, int *index) {
    assert (token);
    assert (index);

    TreeNode *nodeParam1 = nullptr;

    if ((token->topNode + *index)->type == VARIABLE || (token->topNode + *index)->type == NUMBER) {
        TreeNode *node1 = (token->topNode + *index);

        nodeParam1 = (TreeNode *)calloc (1, sizeof (TreeNode));
        ALLOC_DATA_FOR_STR (nodeParam1, "parameter", PARAMETER);

        nodeParam1->right = node1;
        node1->parent = nodeParam1;
        
        (*index)++;

        printf ("data - %s\n", (char *)(token->topNode + *index)->data);

        while (*(char *)(token->topNode + *index)->data == ',') {
            (*index)++;

            TreeNode *node2 = (token->topNode + *index);

            (*index)++;

            TreeNode *nodeParam2 = (TreeNode *)calloc (1, sizeof (TreeNode));
            ALLOC_DATA_FOR_STR (nodeParam2, "parameter", PARAMETER);

            nodeParam2->left = nodeParam1;
            nodeParam2->right = node2;

            node2->parent = nodeParam2;
            nodeParam1->parent = nodeParam2;

            nodeParam1 = nodeParam2;
        }
    }

    return nodeParam1;
}

TreeNode *GetCondition (Token *token, int *index) {
    assert (token);
    assert (index);

    TreeNode *node1 = GetE (token, index);

    char *curSymbol = (char *)(token->topNode + *index)->data;

    if (STR_EQ (">", curSymbol) || STR_EQ ("<", curSymbol) || STR_EQ ("==", curSymbol)) {
        TreeNode *nodeOp = (token->topNode + *index);

        (*index)++;

        TreeNode *node2 = GetE (token, index);

        TreeNode *id = (TreeNode *)calloc (1, sizeof (TreeNode));
        ALLOC_DATA_FOR_STR (id, "condition", CONDITION);

        nodeOp->left = node1;
        nodeOp->right = node2;

        node1->parent = nodeOp;
        node2->parent = nodeOp;

        id->left = nullptr;
        id->right = nodeOp;

        node1 = id;
    }

    return node1;
}

TreeNode *GetStatement (Token *token, int *index) {
    assert (token);
    assert (index);

    TreeNode *node1 = GetAssign (token, index);

    TreeNode *id1 = (TreeNode *)calloc (1, sizeof (TreeNode));
    ALLOC_DATA_FOR_STR (id1, "statement", STATEMENT);

    id1->right = node1;
    node1->parent = id1;
    node1 = id1;

    while (*(char *)token->topNode[*index].data == ';') {
        (*index)++;

        TreeNode *node2 = GetAssign (token, index);

        TreeNode *id2 = (TreeNode *)calloc (1, sizeof (TreeNode));
        ALLOC_DATA_FOR_STR (id2, "statement", STATEMENT);

        node1->parent = id2;

        id2->left = node1;
        id2->right = node2;

        node2->parent = id2;

        node1 = id2;
    }

    return node1;
}

void Require (Token *token, char *symbol, int *index) {
    assert (index);
    assert (token);

    if (!STR_EQ((char *)(token->topNode + *index)->data, symbol)) {
        printf ("Syntax error!\n");
        abort ();
    }

    if ((*index) + 1 < token->numberOfNodes) {
        (*index)++;
    }
}

// int CheckIfVariable (char *str) {
//     assert (str);

//     int length = 0;

//     while (isalpha (**exp)) {
//         str [length] = **exp;
//         (*exp)++;
//         length++;
//     }

//     str [length] = '\0';

//     return length;
// }

void SyntaxError () {
    printf ("Syntax error!\n");

    exit (1);
}
