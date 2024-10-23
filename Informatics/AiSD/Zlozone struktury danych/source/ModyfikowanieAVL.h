#pragma once
#include "Drzewa.h"
#include "RotacjeAVL.h"
#include "WyswietlanieAVL.h"

void dodajWezelAVL(unsigned long long dane)
{
    if (!avl)
    {
        avl = new AVL(dane);
        return;
    }
    AVL* potomek, * point = avl;
    while (true)
    {
        if (point->val <= dane)   //jeżeli wprowadzony węzeł jest równy istniejącemu już, to zostanie wprowadzony po prawej stronie, a drzewko zakwitnie :)
        {
            if (point->right) point = point->right;
            else
            {
                point->right = new AVL(dane, point);
                potomek = point->right;
                break;
            }
        }
        else
        {
            if (point->left) point = point->left;
            else
            {
                point->left = new AVL(dane, point);
                potomek = point->left;
                break;
            }
        }
    }

    if (point->rownowaga) point->rownowaga = 0;
    else
    {
        if (point->left == potomek)  point->rownowaga = 1;
        else    point->rownowaga = -1;
        potomek = point;
        point = point->parent;
        while (point)
        {
            if (point->rownowaga)
            {
                if (point->rownowaga == 1) 
                {
                    if (point->right == potomek) point->rownowaga = 0;
                    else 
                    {
                        if (potomek->rownowaga == -1) 
                        {
                            rotujLewoPrawoAVL(point);
                        }
                        else rotujPrawoAVL(point);
                    }
                }
                else 
                {
                    if (point->left == potomek) point->rownowaga = 0;
                    else 
                    {
                        if (potomek->rownowaga == 1) 
                        {
                            rotujPrawoLewoAVL(point);
                        }
                        else rotujLewoAVL(point);
                    }
                }
                break;
            }

            if (point->left == potomek) point->rownowaga = 1;
            else point->rownowaga = -1;

            potomek = point;
            point = point->parent;
        }
    }
}

void stworzAVL(unsigned long long *dane,  int n )
{
    for (int i = 0; i != n; ++i)
        dodajWezelAVL(dane[i]);
}

void dodajWezelAVL_bezrownowagi(unsigned long long dane , AVL * &korzen = avl)
{

    if (!korzen)
    {
        korzen = new AVL(dane);
        return;
    }
    AVL* point = korzen;
    while (true)
    {
        if (point->val <= dane)   //jeżeli wprowadzony węzeł jest równy istniejącemu już, to zostanie wprowadzony po prawej stronie, a drzewko zakwitnie :)
        {
            if (point->right) point = point->right;
            else
            {
                point->right = new AVL(dane, point);
                break;
            }
        }
        else
        {
            if (point->left) point = point->left;
            else
            {
                point->left = new AVL(dane, point);
                break;
            }
        }
    }
}

void stworzAVL_bezrownowagi(unsigned long long* tablica  ,int start ,int end , bool posortowane_dane = true)
{
    if (!posortowane_dane)
    {
        heapSort(tablica-1 , end);
        stworzAVL_bezrownowagi(tablica-1 , 1 , end , true);
    }
    if(start<=end){
        int m;
        if (start != end) {
            if ((end + 1 - start) % 2)m = ((end + 1 - start) + 1) / 2 + start - 1;
            else m = (end + 1 - start) / 2 + start - 1;
            dodajWezelAVL_bezrownowagi(tablica[m] , avl_binarnie);
            stworzAVL_bezrownowagi(tablica, start, m - 1);
            stworzAVL_bezrownowagi(tablica, m + 1, end);
        } else
            dodajWezelAVL_bezrownowagi(tablica[start] , avl_binarnie);
    }
}

void karczujAVL(AVL* drzewko = avl)
{
    if (!drzewko) return;
    karczujAVL(drzewko->left);
    karczujAVL(drzewko->right);
    delete drzewko;
}

void wykasujAVL(AVL* drzewko , AVL *& korzen)
{
    AVL* toRownowaga;
    AVL* topodRownowaga;
    if (drzewko->right && drzewko->left)
    {
        AVL* toDelete = drzewko->left;
        while (toDelete->right) toDelete = toDelete->right;
        drzewko->val = toDelete->val;
        toRownowaga = toDelete->parent;
        topodRownowaga = toDelete;
        if (toDelete->left)
        {
            toDelete->rownowaga = 0;
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
            if (drzewko == korzen) korzen = drzewko->right;
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
            if (drzewko == korzen) korzen = drzewko->left;
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
            {
                korzen = nullptr;
                delete drzewko;
                return;
            }
        }
        toRownowaga = drzewko->parent;
        topodRownowaga = drzewko;
        delete drzewko;
    }
    while (toRownowaga)
    {
        if(toRownowaga->rownowaga == 0)
        {
            if (toRownowaga->left == topodRownowaga)toRownowaga->rownowaga = -1;
            else toRownowaga->rownowaga = 1;
            return;
        }
        else
        {
            if (toRownowaga->rownowaga == 1)
            {
                if(!toRownowaga->left || toRownowaga->left == topodRownowaga) {
                    toRownowaga->rownowaga = 0;
                    topodRownowaga = toRownowaga;
                    toRownowaga = toRownowaga->parent;
                }
                else
                {
                    if (toRownowaga->left->rownowaga == 0)
                    {
                        rotujPrawoAVL(toRownowaga);
                        return;
                    }
                    else {
                        if (toRownowaga->left->rownowaga == toRownowaga->rownowaga) {
                            rotujPrawoAVL(toRownowaga);
                            topodRownowaga=toRownowaga->parent;
                            toRownowaga=toRownowaga->parent->parent;
                        }
                        else
                        {
                            rotujLewoPrawoAVL(toRownowaga);
                            topodRownowaga=toRownowaga->parent;
                            toRownowaga=toRownowaga->parent->parent;
                        }
                    }
                }
            }
            else
            {
                if(!toRownowaga->right ||toRownowaga->right == topodRownowaga) {
                    toRownowaga->rownowaga = 0;
                    topodRownowaga = toRownowaga;
                    toRownowaga = toRownowaga->parent;
                }
                else
                {
                    if (toRownowaga->right->rownowaga== 0)
                    {
                        rotujLewoAVL(toRownowaga);
                        return;
                    }
                    else {
                        if (toRownowaga->right->rownowaga == toRownowaga->rownowaga) {
                            rotujLewoAVL(toRownowaga);
                            topodRownowaga=toRownowaga->parent;
                            toRownowaga=toRownowaga->parent->parent;
                        }
                        else
                        {
                            rotujPrawoLewoAVL(toRownowaga);
                            topodRownowaga=toRownowaga->parent;
                            toRownowaga=toRownowaga->parent->parent;
                        }
                    }
                }
            }
        }
    }
}

void usunAVL(unsigned long long dane, AVL* drzewko = avl , AVL*& korzen = avl)
{
    if (!drzewko) {
        cout << "Nie znaleziono wezła" << endl;
        return;
    }
    if (drzewko->val == dane)
    {
        wykasujAVL(drzewko, korzen);
        return;
    }
    if (drzewko->val > dane) {
        drzewko = drzewko->left;
    }
    else if (drzewko->val < dane) drzewko = drzewko->right;
    else return;
    usunAVL( dane , drzewko , korzen);
}

void wykasujAVL_bezrownowagi(AVL* drzewko , AVL*& korzen)
{
    if (drzewko->right && drzewko->left)
    {
        AVL* toDelete = drzewko->left;
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
            if (drzewko == korzen) korzen = drzewko->right;
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
            if (drzewko == korzen) korzen = drzewko->left;
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
                korzen = nullptr;
        }
        delete drzewko;
    }
}

void usunAVL_bezrownowagi(unsigned long long dane, AVL* drzewko = avl , AVL*& korzen = avl)
{
    if (!drzewko) {
        cout << "Nie znaleziono wezła" << endl;
        return;
    }
    if (drzewko->val == dane)
    {
        wykasujAVL_bezrownowagi(drzewko , korzen);
        cout << "re" << endl;
        return;
    }
    if (drzewko->val > dane) 
    {
        drzewko = drzewko->left;
    } 
    else drzewko = drzewko->right;
    usunAVL_bezrownowagi( dane , drzewko , korzen);

}
