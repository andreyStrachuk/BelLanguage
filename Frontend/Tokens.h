#pragma once

#include <stdio.h>

#include "../lib/Tree/Tree.h"

struct Token {
    TreeNode *topNode;

    int numberOfNodes;
};

struct MemoryController {
    TreeNode **adr;

    int size;
};

void GetTokens (const char *fileName, Token *tokens);

int CheckForKeyWord (char *src);

int GetTypeOfNode (char *src);

void SetName (TreeNode *node, char *src);

int GetNumberOfNode (char *src);

void SeparateText (Token *token, char *src);

void TreeDestructor (Token *token, MemoryController *mem);

void MemControllerCtor (Token *token, MemoryController *mem);