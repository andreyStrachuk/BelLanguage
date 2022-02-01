#pragma once

#include <stdio.h>

#include "../lib/Tree/Tree.h"

struct Token {
    TreeNode *topNode;

    int numberOfNodes;
};

enum ResOfCheck {
    TRUE = -20,
    FALSE = -19
};

void GetTokens (const char *fileName, Token *tokens);

int CheckForKeyWord (char *src);

int GetTypeOfNode (char *src);

void SetName (TreeNode *node, char *src);

int GetNumberOfNode (char *src);

void SeparateText (Token *token, char *src);