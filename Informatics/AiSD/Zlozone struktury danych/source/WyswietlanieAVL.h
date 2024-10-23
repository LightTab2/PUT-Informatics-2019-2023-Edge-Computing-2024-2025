#pragma once
#include "Drzewa.h"
#include "WyswietlanieBST.h"

void rysujAVL(string galaz = "", string znak = "", AVL* drzewko = avl , bool rownowaga = true)
{
    if (!drzewko) return;
    string out = galaz;
    if (znak == znakiDoRysowania[0]) out[out.length() - 2] = ' ';
    rysujAVL(out + znakiDoRysowania[2], znakiDoRysowania[0], drzewko->right, rownowaga);

    cout << (out.substr(0, galaz.length() - 2)) << znak << drzewko->val; if(rownowaga) cout << '[' << drzewko->rownowaga << ']'; cout << endl;
    out = galaz;

    if (znak == znakiDoRysowania[1]) out[out.length() - 2] = ' ';
    rysujAVL(out + znakiDoRysowania[2], znakiDoRysowania[1], drzewko->left , rownowaga);
}

void wypiszInOrderAVL(AVL* drzewko = avl)
{
    if (!drzewko) return;
    wypiszInOrderAVL(drzewko->left);
    if (!test) cout << drzewko->val << " ";
    wypiszInOrderAVL(drzewko->right);
}

void wypiszPreOrderAVL(AVL* drzewko = avl)
{
    if (!drzewko) return;
    cout << drzewko->val << " ";
    wypiszPreOrderAVL(drzewko->left);
    wypiszPreOrderAVL(drzewko->right);
}

unsigned long long minAVL(AVL* point = avl, bool rownowaga = true)
{
    if (!test)
    {
        string out = "";
        if (!test) 
        {
            cout << point->val;
            if (rownowaga) cout << '[' << point->rownowaga << ']'; cout << endl;
        }
        while (point->left)
        {
            point = point->left;
            if (!test)
            {
                cout << out << znakiDoRysowania[1] << point->val;
                if (rownowaga) cout << '[' << point->rownowaga << ']'; cout << endl;
            }
            out += "  ";
        }
    }
    else
        while (point->left) point = point->left;
    return point->val;
}

unsigned long long maxAVL(string out = "", AVL* point = avl, bool rownowaga = true)
{;
    unsigned long long ret = 0;
    if (point->right)
        ret = maxAVL(out + "  ", point->right, rownowaga);
    else
        return point->val;
    if (!test)
    {
        cout << out << znakiDoRysowania[0] << point->right->val;
        if (rownowaga) cout << '[' << point->rownowaga << ']'; cout << endl;
    }
    return ret;
}