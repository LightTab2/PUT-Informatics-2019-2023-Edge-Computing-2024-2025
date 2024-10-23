#pragma once
#include "Drzewa.h"

unsigned long long pow2log2(unsigned long long x)
{
    unsigned long long y = 1;
    while ((x >>= 1) > 0) y <<= 1;
    return y;
}

bool rotujPrawoBST(BST* wezel_A)
{
    if (!wezel_A->left)
        return false;
    BST* wezel_B = wezel_A->left;
    BST* wezel_nad = wezel_A->parent;
    wezel_A->left = wezel_B->right;
    if (wezel_A->left) wezel_A->left->parent = wezel_A;
    wezel_A->parent = wezel_B;
    wezel_B->parent = wezel_nad;
    wezel_B->right = wezel_A;
    if (wezel_nad) {
        if (wezel_nad->right == wezel_A)
            wezel_nad->right = wezel_B;
        else
            wezel_nad->left = wezel_B;
    }
    else
        bst = wezel_B;
    return true;
}

void rotujLewoBST(BST* wezel_A)
{
    if (!wezel_A->right)
        return;
    BST* wezel_B = wezel_A->right;
    BST* wezel_nad = wezel_A->parent;
    wezel_A->right = wezel_B->left;
    if (wezel_A->right) wezel_A->right->parent = wezel_A;
    wezel_A->parent = wezel_B;
    wezel_B->left = wezel_A;
    wezel_B->parent = wezel_nad;
    if (wezel_nad != nullptr) {
        if (wezel_nad->right == wezel_A)
            wezel_nad->right = wezel_B;
        else
            wezel_nad->left = wezel_B;
    }
    else
        bst = wezel_B;
}

void DSW_BST()
{
    BST* wezel = bst;
    unsigned long long n_wezly = 0;
    while (true)
    {
        while (rotujPrawoBST(wezel)) wezel = wezel->parent;
        n_wezly++;
        if (wezel->right != nullptr)
            wezel = wezel->right;
        else
            break;
    }
    wezel = bst;
    unsigned long long obroty = n_wezly + 1 - pow2log2(n_wezly);
    for (unsigned long long i = 0; i < obroty; i++)
    {
        rotujLewoBST(wezel);
        if (!wezel->parent) break;
        wezel = wezel->parent->right;
    }

    n_wezly = n_wezly - obroty;
    while (n_wezly > 1)
    {
        n_wezly /= 2;
        wezel = bst;
        for (unsigned long long i = 0; i < n_wezly; i++)
        {
            rotujLewoBST(wezel);
            if (!wezel->parent) break;
            wezel = wezel->parent->right;
        }
    }
}