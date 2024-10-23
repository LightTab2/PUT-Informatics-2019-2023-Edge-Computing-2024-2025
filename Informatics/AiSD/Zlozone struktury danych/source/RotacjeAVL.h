#pragma once
#include "Drzewa.h"
#include "WyswietlanieAVL.h"

void rotujPrawoAVL(AVL* wezel_A)
{
    if (wezel_A->left == nullptr)
        return;
    AVL* wezel_B = wezel_A->left;
    AVL* wezel_nad = wezel_A->parent;
    wezel_A->left = wezel_B->right;
    if (wezel_A->left) wezel_A->left->parent = wezel_A;
    wezel_A->parent = wezel_B;
    wezel_B->parent = wezel_nad;
    wezel_B->right = wezel_A;
    if (wezel_nad != nullptr) {
        if (wezel_nad->right == wezel_A)
            wezel_nad->right = wezel_B;
        else
            wezel_nad->left = wezel_B;
    }
    else
        avl = wezel_B;

    if (wezel_A->parent->rownowaga == 1)
    {
        wezel_A->rownowaga = 0;
        wezel_A->parent->rownowaga = 0;
    }
    else
    {
        wezel_A->rownowaga = 1;
        wezel_A->parent->rownowaga = -1;
    }
}

void rotujLewoAVL(AVL* wezel_A)
{
    AVL* wezel_B = wezel_A->right;
    AVL* wezel_nad = wezel_A->parent;
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
        avl = wezel_B;
    if (wezel_B->rownowaga == -1) {
        wezel_A->rownowaga = 0;
        wezel_B->rownowaga = 0;
    }
    else {
        wezel_A->rownowaga = -1;
        wezel_B->rownowaga = 1;
    }
}

void rotujLewoPrawoAVL(AVL* wezel_A)
{
    AVL* wezel_B = wezel_A->left;
    AVL* wezel_C = wezel_B->right;
    AVL* wezel_nad = wezel_A->parent;

    wezel_B->parent = wezel_C;
    wezel_B->right = wezel_C->left;
    if (wezel_B->right) wezel_B->right->parent = wezel_B;
    wezel_C->left = wezel_B;

    wezel_A->parent = wezel_C;
    wezel_A->left = wezel_C->right;
    if (wezel_A->left) wezel_A->left->parent = wezel_A;
    wezel_C->right = wezel_A;

    wezel_C->parent = wezel_nad;

    if (wezel_nad != nullptr) {
        if (wezel_nad->right == wezel_A) wezel_nad->right = wezel_C;
        else wezel_nad->left = wezel_C;
    }
    else
        avl = wezel_C;

    if (wezel_C->rownowaga == 1)
        wezel_A->rownowaga = -1;
    wezel_B->rownowaga = 0;

    if (wezel_C->rownowaga == -1)
        wezel_B->rownowaga = 1;
    wezel_A->rownowaga = 0;

    wezel_C->rownowaga = 0;
}

void rotujPrawoLewoAVL(AVL* wezel_A)
{
    AVL* wezel_B = wezel_A->right;
    AVL* wezel_C = wezel_B->left;
    AVL* wezel_nad = wezel_A->parent;
    wezel_B->parent = wezel_C;
    wezel_B->left = wezel_C->right;
    if (wezel_B->left) wezel_B->left->parent = wezel_B;
    wezel_C->right = wezel_B;

    wezel_A->parent = wezel_C;
    wezel_A->right = wezel_C->left;
    if (wezel_A->right) wezel_A->right->parent = wezel_A;
    wezel_C->left = wezel_A;

    wezel_C->parent = wezel_nad;

    if (wezel_nad) {
        if (wezel_nad->left == wezel_A) wezel_nad->left = wezel_C;
        else wezel_nad->right = wezel_C;
    }
    else avl = wezel_C;

    if (wezel_C->rownowaga == 1)
        wezel_B->rownowaga = -1;
    wezel_A->rownowaga = 0;
    if (wezel_C->rownowaga == -1)
        wezel_A->rownowaga = 1;
    wezel_B->rownowaga = 0;

    wezel_C->rownowaga = 0;
}