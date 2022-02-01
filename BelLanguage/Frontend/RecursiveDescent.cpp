#include "RecDescent.h"  

TreeNode *GetCode (Token *token, int *index, MemoryController *mem) {
    assert (token);
    assert (index);
    assert (mem);

    TreeNode *nodeA = GetAssign (token, index, mem);

    TreeNode *nodeSt1 = (TreeNode *)calloc (1, sizeof (TreeNode));
    ALLOC_DATA_FOR_STR (nodeSt1, "statement", STATEMENT);
    PushNodeToMemArr (mem, nodeSt1);

    nodeSt1->right = nodeA;
    nodeA->parent = nodeSt1;

    while ((token->topNode + *index)->type == VARIABLE) {
        TreeNode *nodeB = GetAssign (token, index, mem);

        TreeNode *nodeSt2 = (TreeNode *)calloc (1, sizeof (TreeNode));
        ALLOC_DATA_FOR_STR (nodeSt2, "statement", STATEMENT);
        PushNodeToMemArr (mem, nodeSt2);

        nodeSt2->left = nodeSt1;
        nodeSt2->right = nodeB;
        nodeSt1->parent = nodeSt2;
        nodeB->parent = nodeSt2;

        nodeSt1 = nodeSt2;
    }

    TreeNode *nodeF = GetFunctions (token, index, mem);

    TreeNode *nodeFCpy = nodeF;

    while (nodeFCpy->left != nullptr) nodeFCpy = nodeFCpy->left;

    nodeFCpy->left = nodeSt1;
    if (nodeSt1 != nullptr) nodeSt1->parent = nodeFCpy;

    return nodeF;
}

TreeNode *GetE (Token *token, int *index, MemoryController *mem) {
    assert (token);
    assert (index);

    TreeNode *node1 = GetT (token, index, mem);

    TreeNode *nodeOp = nullptr;

    while (*(char *)token->topNode[*index].data == '+' || *(char *)token->topNode[*index].data == '-') {
        nodeOp = &(token->topNode[*index]);

        (*index)++;

        TreeNode *node2 = GetT (token, index, mem);

        nodeOp->left = node1;
        nodeOp->right = node2;

        node1->parent = nodeOp;
        node2->parent = nodeOp;

        node1 = nodeOp;
    }

    return node1;
}

TreeNode *GetT (Token *token, int *index, MemoryController *mem) {
    assert (token);
    assert (index);

    TreeNode *node1 = GetExponent (token, index, mem);

    while (*(char *)token->topNode[*index].data == '*' || *(char *)token->topNode[*index].data == '/') {
        TreeNode *nodeOp = &(token->topNode[*index]);

        (*index)++;

        TreeNode *node2 = GetExponent (token, index, mem);

        nodeOp->left = node1;
        nodeOp->right = node2;

        node1->parent = nodeOp;
        node2->parent = nodeOp;

        node1 = nodeOp;
    }

    return node1;
}

TreeNode *GetP (Token *token, int *index, MemoryController *mem) {
    assert (token);
    assert (index);

    if (*(char *)(token->topNode + *index)->data ==  '(') {
        (*index)++;

        TreeNode *node = GetE (token, index, mem);

        Require (token, ")", index);

        return node;

    }
    else if ((token->topNode + *index)->type == VARIABLE) {
        if (*index + 1 < token->numberOfNodes) 
            return (token->topNode + (*index)++);

        return (token->topNode + *index);
    }
    else if (STR_EQ ("vyarnut", (char *)(token->topNode + *index)->data)) {
        TreeNode *nodeCurrent = (token->topNode + *index);

        if (*index + 1 < token->numberOfNodes) 
            (*index)++;
        else {
            return nodeCurrent;
        }

        TreeNode *nodeRetVal = GetN (token, index);

        nodeCurrent->right = nodeRetVal;
        nodeRetVal->parent = nodeCurrent;

        return nodeCurrent;
    }
    else if (*(char *)(token->topNode + *index)->data == '-') {
        (*index)++;

        TreeNode *nodeExp = GetE (token, index, mem);

        TreeNode *nodeHelp1 = (TreeNode *)calloc (1, sizeof (TreeNode));
        ALLOC_DATA_FOR_STR (nodeHelp1, "*", OPERATION);

        PushNodeToMemArr (mem, nodeHelp1);

        TreeNode *nodeHelp2 = (TreeNode *)calloc (1, sizeof (TreeNode));
        PushNumberToNode (nodeHelp2, -1);

        PushNodeToMemArr (mem, nodeHelp2);

        nodeHelp1->left = nodeHelp2;
        nodeHelp1->right = nodeExp;

        nodeHelp2->parent = nodeHelp1;
        nodeExp->parent = nodeHelp2;

        return nodeHelp1;
    }
    else if ((token->topNode + *index)->type == FUNCTION && !STR_EQ("main", (char *)(token->topNode + *index)->data)) {

        return GetFuncCall (token, index, mem);
    }
    else if (STR_EQ ("kali", (char *)(token->topNode + *index)->data) || STR_EQ ("pakul", (char *)(token->topNode + *index)->data)) {
        TreeNode *nodeIf = (token->topNode + *index);
        (*index)++;

        Require (token, "(", index);

        TreeNode *nodeCond = GetCondition (token, index, mem);

        Require (token, ")", index);

        Require (token, "begin", index);

        TreeNode *nodeStatement = GetStatement (token, index, mem);

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

    return node;
}

TreeNode *GetAssign (Token *token, int *index, MemoryController *mem) {
    assert (token);
    assert (index);

    TreeNode *node1 = GetE (token, index, mem);

    if (*(char *)token->topNode[*index].data == '=') {
        if (token->topNode[*index - 1].type != VARIABLE) {
            SyntaxError ();
        }

        TreeNode *nodeOp = &(token->topNode[*index]);

        (*index)++;

        TreeNode *id = (TreeNode *)calloc (1, sizeof (TreeNode));
        ALLOC_DATA_FOR_STR (id, "initializer", INITIALIZER);

        PushNodeToMemArr (mem, id);

        TreeNode *node2 = GetE (token, index, mem);

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

TreeNode *GetExponent (Token *token, int *index, MemoryController *mem) {
    assert (token);
    assert (index);

    TreeNode *node1 = GetP (token, index, mem);

    while (*(char *)token->topNode[*index].data == '^') {
        TreeNode *nodeOp = &(token->topNode[*index]);

        (*index)++;

        TreeNode *node2 = GetP (token, index, mem);

        node1->parent = nodeOp;
        node2->parent = nodeOp;

        nodeOp->left = node1;
        nodeOp->right = node2;

        node1 = nodeOp;
    }

    return node1;
}

TreeNode *GetFuncCall (Token *token, int *index, MemoryController *mem) {
    assert (token);
    assert (index);

    TreeNode *nodeCall = (TreeNode *)calloc (1 ,sizeof (TreeNode));
    ALLOC_DATA_FOR_STR (nodeCall, "call", OPERATION);

    PushNodeToMemArr (mem, nodeCall);

    TreeNode *nodeName = (token->topNode + *index);
    (*index)++;

    Require (token, "(", index);

    TreeNode *args = GetArgs (token, index, mem);

    Require (token, ")", index);

    nodeCall->right = args;
    nodeCall->left = nodeName;

    args->parent = nodeCall;
    nodeName->parent = nodeCall;

    return nodeCall;
}

TreeNode *GetFuncDef (Token *token, int *index, MemoryController *mem) {
    assert (token);
    assert (index);

    TreeNode *nodeDef = nullptr;

    if ((token->topNode + *index)->type == FUNCTION) {
        TreeNode *nodeName = (token->topNode + *index);
        (*index)++;

        Require (token, "(", index);

        TreeNode *args = GetArgs (token, index, mem);

        Require (token, ")", index);

        Require (token, "begin", index);
        
        TreeNode *nodeIn = GetStatement (token, index, mem);

        Require (token, "end", index);

        TreeNode *nodeFunc = (TreeNode *)calloc (1, sizeof (TreeNode));
        ALLOC_DATA_FOR_STR (nodeFunc, "function", FUNCTION);

        PushNodeToMemArr (mem, nodeFunc);

        nodeFunc->left = nodeName;
        if (args != nullptr) nodeFunc->right = args;

        nodeName->parent = nodeFunc;
        if (args != nullptr) args->parent = nodeFunc;

        nodeDef = (TreeNode *)calloc (1, sizeof (TreeNode));
        ALLOC_DATA_FOR_STR (nodeDef, "define", OPERATION);

        PushNodeToMemArr (mem, nodeDef);

        nodeDef->left = nodeFunc;
        nodeDef->right = nodeIn;

        nodeIn->parent = nodeDef;
        nodeIn->parent = nodeDef;
    }

    return nodeDef;
}

TreeNode *GetFunctions (Token *token, int *index, MemoryController *mem) {
    assert (token);
    assert (index);

    TreeNode *node1 = GetFuncDef (token, index, mem);

    TreeNode *nodeSt = (TreeNode *)calloc (1, sizeof (TreeNode));
    ALLOC_DATA_FOR_STR (nodeSt, "statement", STATEMENT);

    PushNodeToMemArr (mem, nodeSt);

    nodeSt->right = node1;
    node1->parent = nodeSt;

    while ((token->topNode + *index)->type == FUNCTION) {
        TreeNode *node2 = GetFuncDef (token, index, mem);

        TreeNode *nodeSt2 = (TreeNode *)calloc (1, sizeof (TreeNode));
        ALLOC_DATA_FOR_STR (nodeSt2, "statement", STATEMENT);

        PushNodeToMemArr (mem, nodeSt2);

        nodeSt2->right = node2;
        nodeSt2->left = nodeSt;

        node2->parent = nodeSt2;
        node1->parent = nodeSt2;

        nodeSt = nodeSt2;
    }

    return nodeSt;
}

TreeNode *GetArgs (Token *token, int *index, MemoryController *mem) {
    assert (token);
    assert (index);

    TreeNode *nodeParam1 = nullptr;

    if ((token->topNode + *index)->type == VARIABLE || (token->topNode + *index)->type == NUMBER) {
        TreeNode *node1 = (token->topNode + *index);

        nodeParam1 = (TreeNode *)calloc (1, sizeof (TreeNode));
        ALLOC_DATA_FOR_STR (nodeParam1, "parameter", PARAMETER);

        PushNodeToMemArr (mem, nodeParam1);

        nodeParam1->right = node1;
        node1->parent = nodeParam1;
        
        (*index)++;

        while (*(char *)(token->topNode + *index)->data == ',') {
            (*index)++;

            TreeNode *node2 = (token->topNode + *index);

            (*index)++;

            TreeNode *nodeParam2 = (TreeNode *)calloc (1, sizeof (TreeNode));
            ALLOC_DATA_FOR_STR (nodeParam2, "parameter", PARAMETER);

            PushNodeToMemArr (mem, nodeParam2);

            nodeParam2->left = nodeParam1;
            nodeParam2->right = node2;

            node2->parent = nodeParam2;
            nodeParam1->parent = nodeParam2;

            nodeParam1 = nodeParam2;
        }
    }

    return nodeParam1;
}

TreeNode *GetCondition (Token *token, int *index, MemoryController *mem) {
    assert (token);
    assert (index);

    TreeNode *node1 = GetE (token, index, mem);

    char *curSymbol = (char *)(token->topNode + *index)->data;

    if (STR_EQ (">", curSymbol) || STR_EQ ("<", curSymbol) || STR_EQ ("==", curSymbol)) {
        TreeNode *nodeOp = (token->topNode + *index);

        (*index)++;

        TreeNode *node2 = GetE (token, index, mem);

        TreeNode *id = (TreeNode *)calloc (1, sizeof (TreeNode));
        ALLOC_DATA_FOR_STR (id, "condition", CONDITION);

        PushNodeToMemArr (mem, id);

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

TreeNode *GetStatement (Token *token, int *index, MemoryController *mem) {
    assert (token);
    assert (index);

    TreeNode *node1 = GetAssign (token, index, mem);

    TreeNode *id1 = (TreeNode *)calloc (1, sizeof (TreeNode));
    ALLOC_DATA_FOR_STR (id1, "statement", STATEMENT);

    PushNodeToMemArr (mem, id1);

    id1->right = node1;
    node1->parent = id1;
    node1 = id1;

    while (*(char *)token->topNode[*index].data == ';') {
        (*index)++;

        TreeNode *node2 = GetAssign (token, index, mem);

        TreeNode *id2 = (TreeNode *)calloc (1, sizeof (TreeNode));
        ALLOC_DATA_FOR_STR (id2, "statement", STATEMENT);

        PushNodeToMemArr (mem, id2);

        node1->parent = id2;

        id2->left = node1;
        id2->right = node2;

        node2->parent = id2;

        node1 = id2;
    }

    return node1;
}

void SyntaxError () {
    printf ("Syntax error!\n");

    exit (1);
}

void PushNodeToMemArr (MemoryController *mem, TreeNode *node) {
    assert (mem);
    assert (node);

    int tmp = mem->size;

    mem->adr[tmp] = node;
    mem->size++;
}

void Require (Token *token, const char *symbol, int *index) {
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
