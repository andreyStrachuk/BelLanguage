#ifndef CALCULATOR_H
#define CALCULATOR_H

#define ASSERT_OK(smth, doSmth)         do { if (smth) { doSmth;}} while (0)

#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <math.h>

#include "Tokens.h"


#define STR_EQ(str1, str2)             (strncmp (str1, str2, strlen (str1)) == 0)

// enum FUNCTION_TYPES {
//     NOTFOUND,
//     COS,
//     SIN,
//     LN,
//     SQRT
// };

TreeNode *GetP (Token *token, int *index, MemoryController *mem);

TreeNode *GetCode (Token *token, int *index, MemoryController *mem);

TreeNode *GetFuncDef (Token *token, int *index, MemoryController *mem);

TreeNode *GetArgs (Token *token, int *index, MemoryController *mem);

TreeNode *GetFunctions (Token *token, int *index, MemoryController *mem);

void SyntaxError ();

TreeNode *GetN (Token *token, int *index);

TreeNode *GetG (Token *token, int *index, MemoryController *mem);

TreeNode *GetE (Token *token, int *index, MemoryController *mem);

TreeNode *GetT (Token *token, int *index, MemoryController *mem);

TreeNode *GetExponent (Token *token, int *index, MemoryController *mem);

TreeNode *GetAssign (Token *token, int *index, MemoryController *mem);

TreeNode *GetStatement (Token *token, int *index, MemoryController *mem);

TreeNode *GetCondition (Token *token, int *index, MemoryController *mem);

TreeNode *GetFuncCall (Token *token, int *index, MemoryController *mem);

void PushNodeToMemArr (MemoryController *mem, TreeNode *node);

void Require (Token *token, const char *symbol, int *index);

#endif
