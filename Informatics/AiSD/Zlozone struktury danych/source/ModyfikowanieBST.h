#pragma once
#include "Drzewa.h"

void dodajWezelBST(unsigned long long dane , BST * &korzen = bst)
{
    if (!korzen)
    {
        korzen = new BST(dane);
        return;
    }
    BST* point = korzen;
    while (true)
    {
        if (point->val <= dane)   //jeżeli wprowadzony węzeł jest równy istniejącemu już, to zostanie wprowadzony po prawej stronie, a drzewko zakwitnie :)
        {
            if (point->right) point = point->right;
            else
            {
                point->right = new BST(dane, point);
                break;
            }
        }
        else
        {
            if (point->left) point = point->left;
            else
            {
                point->left = new BST(dane, point);
                break;
            }
        }
    }
}

void stworzBST(unsigned long long *dane,  int n )
{
    for (int i = 0; i != n; ++i)
        dodajWezelBST(dane[i]);
}

void wykasujBST(BST* drzewko)
{
    if (drzewko->right && drzewko->left)
    {
        BST* toDelete = drzewko->left;
        while (toDelete->right) toDelete = toDelete->right;
        drzewko->val = toDelete->val;
        if (toDelete->left)
        {
            toDelete->val = toDelete->left->val;
            toDelete = toDelete->left;
            toDelete->parent->left = toDelete->left;
        }
        if (toDelete->parent->right == toDelete) toDelete->parent->right = nullptr;
        else toDelete->parent->left = nullptr;
        delete toDelete;
    }
    else
    {
        if (drzewko->right) 
        {
            drzewko->right->parent = drzewko->parent;
            if (drzewko->parent) 
            {
                if (drzewko->parent->left == drzewko)
                    drzewko->parent->left = drzewko->right;
                else
                    drzewko->parent->right = drzewko->right;
            }
            if (drzewko == bst) bst = drzewko->right;
        } 
        else if (drzewko->left) 
        {
            drzewko->left->parent = drzewko->parent;
            if (drzewko->parent) 
            {
                if (drzewko->parent->left == drzewko)
                    drzewko->parent->left = drzewko->left;
                else
                    drzewko->parent->right = drzewko->left;
            }
            if (drzewko == bst) bst = drzewko->left;
        } 
        else 
        {
            if (drzewko->parent) 
            {
                if (drzewko->parent->left == drzewko)
                    drzewko->parent->left = nullptr;
                else
                    drzewko->parent->right = nullptr;
            }
            else
                bst = nullptr;
        }
        delete drzewko;
    }
}

void usunBST(unsigned long long dane, BST* drzewko = bst)
{
    if (!drzewko) {
        cout << "Nie znaleziono wezła" << endl;
        return;
    }
    if (drzewko->val == dane)
    {
        wykasujBST(drzewko);
        return;
    }
    if (drzewko->val > dane) 
    {
        drzewko = drzewko->left;
    } 
    else drzewko = drzewko->right;
    usunBST( dane , drzewko);
}

void karczujBST(BST* drzewko = bst)
{
    if (!drzewko) return;
    karczujBST(drzewko->left);
    karczujBST(drzewko->right);
    delete drzewko;
}
