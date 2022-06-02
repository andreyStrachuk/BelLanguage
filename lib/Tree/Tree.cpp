#include "Tree.h"

void DestructTree (TreeNode *topNode) {
    assert (topNode);

    free (topNode->data);

    if (topNode->right == nullptr) {

        return;
    }

    if (topNode->left != nullptr) {
        DestructTree (topNode->left);

        free (topNode->left);
    }

    DestructTree (topNode->right);

    free (topNode->right);
}

void PushNumberToNode (TreeNode *node, const double val) {
    assert (node);

    if (node->data == nullptr) {
        node->data = calloc (1, sizeof (double));
    }
    else {
        node->data = realloc (node->data, sizeof (double));
    }

    memcpy (node->data, (void *)&val, sizeof (double));
    node->type = NUMBER;
}

void DumpTree (TreeNode *node, FILE *treeDump) {
    assert (node);

    fprintf (treeDump, "digraph D {\n");

    fprintf (treeDump, "    %lu [fillcolor=purple, style=\"rounded,filled\", label=\"", (unsigned long)node % 65536);
    DumpData (node, treeDump);
    fprintf (treeDump, "\"];\n");

    if (node->left != nullptr) {
        PreOrdSearch(node, node->left, treeDump);
    } 
    if (node->right != nullptr) {
        PreOrdSearch (node, node->right, treeDump);
    }
    
    fprintf (treeDump, "}");

    fclose (treeDump);

    system ("dot -Tpng DumpTree.txt -O\n"
            "xdg-open DumpTree.txt.png\n");
}

void PreOrdSearch (TreeNode *node, TreeNode *childNode, FILE *file) {
    assert (node);
    assert (childNode);

    if (childNode == node->left) {
        fprintf (file, "    %lu -> %lu;\n\n", (unsigned long)node % 65536, (unsigned long)childNode % 65536);
    }
    if (childNode == node->right) {
        fprintf (file, "    %lu -> %lu [ color = red ];\n\n", (unsigned long)node % 65536, (unsigned long)childNode % 65536);
    }
    // fprintf (file, "    %lu -> %lu;\n\n", (unsigned long)childNode % 65536, (unsigned long)childNode->parent % 65536);

    if (childNode->right == nullptr && childNode->left == nullptr) {
        fprintf (file, "    %lu [fillcolor=yellow, shape=box, style=\"filled\", label=\"", (unsigned long)childNode % 65536);
        DumpData (childNode, file);
        fprintf (file, "\"];\n");

        return;
    }

    fprintf (file, "    %lu [fillcolor=cyan, style=\"rounded,filled\", label=\"", (unsigned long)childNode % 65536);
    DumpData (childNode, file);
    fprintf (file, "\"];\n");

    if (childNode->left != nullptr) {
        PreOrdSearch (childNode, childNode->left, file);
    }

    if (childNode->right != nullptr) {
        PreOrdSearch (childNode, childNode->right, file);
    }

    return;
}

void DumpData (TreeNode *node, FILE *dump) {
    assert (node);

    switch (node->type) {
        case NUMBER: {
            fprintf (dump, "%lg", *(double *)node->data);

            break;
        }
        default: {
            fprintf (dump, "%s", (char *)node->data);
        }
    }
}

#define ASSERT_OK(smth, doSmth)         do { if (smth) { doSmth;}} while (0)

int PrintTreeToFile (TreeNode *node, FILE *file) {
    assert (node);
    assert (file);

    int res = 0;

    if (node->right == nullptr) {
        fprintf (file, "( ");
        PrintDataToFile (node, file);
        fprintf (file, " ) ");

        return OK;
    }

    fprintf (file, "( ");

    if (node->left != nullptr) {
        res = PrintTreeToFile (node->left, file);
        ASSERT_OK(res != OK, PrintErrors (res); return res);
    }

    PrintDataToFile (node, file);
    fprintf (file, " ");

    if (node->right != nullptr) {
        res = PrintTreeToFile (node->right, file);
        ASSERT_OK(res != OK, PrintErrors (res); return res);
    }

    fprintf (file, ") ");

    return OK;
}

void PrintDataToFile (TreeNode *node, FILE *file) {
    assert (node);
    assert (file);

    if (node->type == NUMBER) {
        fprintf (file, "%d %lg ", node->type, *(double *)node->data);
    }
    else {
        fprintf (file, "%d %s ", node->type, (char *)node->data);
    }
}

int DoubleComp (const double val1, const double val2) {
    if (fabs(val1 - val2) < EPS) {
        return EQUAL;
    }

    if (val1 > val2) {
        return BIGGER;
    }

    return LESS;
}

#undef ASSERT_OK