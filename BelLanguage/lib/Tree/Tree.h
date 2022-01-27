#pragma once

#include <stdio.h>
#include <assert.h>
#include <stdlib.h>

#include "../Text/TextAnalyze.h"

struct TreeNode {
    char type;

    void *data;

    TreeNode *left;
    TreeNode *right;
    TreeNode *parent;
};

enum TypeOfToken {
    KEYWORD,
    IDENTIFIER,
    STATEMENT,
    NUMBER,
    VARIABLE,
    PARENTHESIS,
    FUNCTION,
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