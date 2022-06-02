#pragma once

#include <stdio.h>

#include "../lib/Text/TextAnalyze.h"
#include "../lib/Tree/Tree.h"

struct Variable {
    double value;
    char *name;
    int shift;
};

struct VarsTable {
    int startAddr;

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

void GetTreeVal (TreeNode *node, VarsTable *table, FILE *file);

int GetVarIndex (TreeNode *node, VarsTable *table);

void AssembleStatement (TreeNode *node, VarsTable *table, FILE *file, const int widthOfStackFrame);

int GetResOfCondition (TreeNode *node, VarsTable *table, const double val1, const double val2);

void FPrintJump (TreeNode *node, FILE *file);

int GetVarAddr (TreeNode *node, VarsTable *table);

int AssemblePrintFunction (TreeNode *node, VarsTable *table, FILE *file);

int AssembleScanFunction (TreeNode *node, VarsTable *table, FILE *file);

int AssembleSqrtFunction (TreeNode *node, VarsTable *table, FILE *file);

int GetNumberOfParams (TreeNode *node);

void AssembleFunctions (TreeNode *node, FILE *file);

int FillParametersToTable (TreeNode *node, VarsTable *table, FILE *file);

int LoadParamsToRAM (TreeNode *node, VarsTable *table, FILE *file);