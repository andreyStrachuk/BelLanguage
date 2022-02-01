#pragma once

#include <stdio.h>
#include <assert.h>
#include <stdlib.h>

#include "../Text/TextAnalyze.h"

struct TreeNode {
    TreeNode *left;
    TreeNode *right;
    TreeNode *parent;

    char type;
    char number;

    void *data;
};

enum TypeOfToken {
    KEYWORD,
    INITIALIZER,
    STATEMENT,
    NUMBER,
    VARIABLE,
    PARENTHESIS,
    FUNCTION,
    MAIN,
    CONDITION,
    OPERATION,
    END_OF_STR,
    PARAMETER
};

#define ALLOC_DATA_FOR_STR(diffNode, oper, type_t)          diffNode->data = calloc (strlen (oper) + 1, sizeof (char));  \
                                                            diffNode->type = type_t;                                     \
                                                            memcpy (diffNode->data, (void *)(&oper), strlen (oper) + 1);

void DestructTree (TreeNode *topNode);

void DestructNode (TreeNode *node);

void PushNumberToNode (TreeNode *node, const double val);

void DumpTree (TreeNode *node, FILE *treeDump);

void PreOrdSearch (TreeNode *node, TreeNode *childNode, FILE *file);

void DumpData (TreeNode *node, FILE *dump);