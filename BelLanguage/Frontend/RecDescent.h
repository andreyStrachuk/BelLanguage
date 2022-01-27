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

enum FUNCTION_TYPES {
    NOTFOUND,
    COS,
    SIN,
    LN,
    SQRT
};

TreeNode *GetP (Token *token, int *index);

TreeNode *GetFuncDef (Token *token, int *index);

TreeNode *GetArgs (Token *token, int *index);

void Require (Token *token, char *symbol, int *index);

int GetString (char *str, char **exp);

void SyntaxError ();

TreeNode *GetN (Token *token, int *index);

TreeNode *GetG (Token *token, int *index);

TreeNode *GetE (Token *token, int *index);

TreeNode *GetT (Token *token, int *index);

TreeNode *GetExponent (Token *token, int *index);

TreeNode *GetAssign (Token *token, int *index);

TreeNode *GetStatement (Token *token, int *index);

TreeNode *GetCondition (Token *token, int *index);

#endif
