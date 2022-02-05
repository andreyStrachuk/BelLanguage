#pragma once

#include <stdio.h>

#include "../lib/Text/TextAnalyze.h"
#include "../lib/Tree/Tree.h"

struct Variable {
    double value;
    int addr;
    char *name;
};

struct VarsTable {
    Variable *vars;

    int size;
    int free;
};

int GetTree (char *exp, TreeNode **topNode);

void GetApproxNumberOfVars (TreeNode *topNode, int *varsNum);

int CheckIfVarInTable (const char *str, VarsTable *table);

int FillVarsTable (TreeNode *node, VarsTable *table);

int ASTtoAsm (TreeNode *node, VarsTable *table);

double GetVarVal (TreeNode *node, VarsTable *table);

double GetTreeVal (TreeNode *node, VarsTable *table);

int GetVarIndex (TreeNode *node, VarsTable *table);

void AssembleStatement (TreeNode *node, VarsTable *table, FILE *file);

int GetResOfCondition (TreeNode *node, VarsTable *table, const double val1, const double val2);

void FPrintJump (TreeNode *node, FILE *file);