#pragma once
#include "Drzewa.h"
string maxULL = to_string(static_cast<unsigned long long>(-1)), maxU = to_string(static_cast<unsigned>(-1));
string znakiDoRysowania[] = { string(1, char(218)) + char(196), string(1, char(192)) + char(196), string(1, char(179)) + ' ' };
bool test = false, file = false;

void rysujBST(string galaz = "", string znak = "", BST* drzewko = bst) //procedura na stronie https://eduinf.waw.pl/inf/alg/001_search/0112.php bardzo mi się spodobała, więc pomysł zapożyczyłem
{
    if (!drzewko) return;
    string out = galaz;
    if (znak == znakiDoRysowania[0]) out[out.length() - 2] = ' ';
    rysujBST(out + znakiDoRysowania[2], znakiDoRysowania[0], drzewko->right);

    cout << (out.substr(0, galaz.length() - 2)) << znak << drzewko->val << endl;
    out = galaz;

    if (znak == znakiDoRysowania[1]) out[out.length() - 2] = ' ';
    rysujBST(out + znakiDoRysowania[2], znakiDoRysowania[1], drzewko->left);
}

void wypiszInOrderBST(BST* drzewko = bst)
{
    if (!drzewko) return;
    wypiszInOrderBST(drzewko->left);
    if (!test) cout << drzewko->val << " ";
    wypiszInOrderBST(drzewko->right);
}

void wypiszPreOrderBST(BST* drzewko = bst)
{
    if (!drzewko) return;
    cout << drzewko->val << " ";
    wypiszPreOrderBST(drzewko->left);
    wypiszPreOrderBST(drzewko->right);
}

unsigned long long minBST()
{
    BST* point = bst;
    if (!test)
    {
        string out = "";
        cout << point->val << endl;
        while (point->left)
        {
            point = point->left;
            cout << out << znakiDoRysowania[1] << point->val << endl;
            out += "  ";
        }
    }
    else
        while (point->left) point = point->left;
    return point->val;
}

unsigned long long maxBST(string out = "", BST* point = bst)
{
    unsigned long long ret = 0;
    if (point->right)
        ret = maxBST(out + "  ", point->right);
    else
        return point->val;
    if (!test)
        cout << out << znakiDoRysowania[0] << point->right->val << endl;
    return ret;
}
