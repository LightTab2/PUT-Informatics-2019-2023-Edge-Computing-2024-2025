#pragma once
#include "ZmienneGlobalne.h"

bool DFS_macierz(unsigned element)
{
    for (unsigned i = 0; i != rozmiarDanych; ++i)
        if (macierzSasiedztwa[element][i] == 1)
        {
            if (odwiedzone[i])
                return true;
            else
            {
                odwiedzone[i] = true;
                if (DFS_macierz(i))
                {
                    odwiedzone[i] = false;
                    return true;
                }
                odwiedzone[i] = false;
            }
        }
    return false;
}

bool DFS_lista(unsigned element)
{
    for (auto it = listaNastepnikow[element].begin(); it != listaNastepnikow[element].end(); ++it)
    {
        if (odwiedzone[*it])
            return true;
        else
        {
            odwiedzone[*it] = true;
            if (DFS_lista(*it))
            {
                odwiedzone[*it] = false;
                return true;
            }
            odwiedzone[*it] = false;
        }
    }
    return false;
}
