#pragma once
#include "ZmienneGlobalne.h"
//W tym pliku jest kod związany z wyświetlaniem, dlatego że nie jest on najistotniejszy, nie został obarczony komentarzem

void linia(int dlugosc, int mode = 0)
{
    if (!mode)
    {
        cout << (char)195 << (char)196 << (char)196 << (char)196;
        for (int i = 4; i < dlugosc; i += 4)
            cout << (char)197 << (char)196 << (char)196 << (char)196;
        cout << (char)180;
    }
    else if (mode == 1)
    {
        cout << (char)218 << (char)196 << (char)196 << (char)196;
        for (int i = 4; i < dlugosc; i += 4)
            cout << (char)194 << (char)196 << (char)196 << (char)196;
        cout << (char)191;
    }
    else
    {
        cout << (char)192 << (char)196 << (char)196 << (char)196;
        for (int i = 4; i < dlugosc; i += 4)
            cout << (char)193 << (char)196 << (char)196 << (char)196;
        cout << (char)217;
    }
    cout << endl;
}

void rysujMacierz(short int** const &macierz, bool macierzGrafu = false, unsigned wiersze = rozmiarDanych)
{
    if (wiersze > 50) return;
    unsigned kolumny = wiersze + macierzGrafu * 3;

    if (!wiersze) return;
    linia((kolumny << 2u), 1);
    for (unsigned i = 0; i != wiersze;)
    {
        for (unsigned j = 0; j != kolumny; ++j)
        {
            bool bHasMinus = (to_string(macierz[i][j])[0] == '-') && (!(to_string(macierz[i][j]).size() % 2));
            cout << (char)179;
            for (int x = 0; x < (2 - ((int)to_string(macierz[i][j]).size() + 1) / 2 - bHasMinus); ++x) cout << " ";
            cout << (int)macierz[i][j];
            for (int x = 0; x < (1 - ((int)to_string(macierz[i][j]).size()) / 2 + bHasMinus); ++x) cout << " ";
        }
        cout << (char)179;
        cout << endl;
        if (++i == rozmiarDanych) break;
        linia((kolumny << 2u));
    }
    linia((kolumny << 2u), 2);
    cout << endl;
}

void rysujNastepnikow()
{

    if (rozmiarDanych > 50) return;
    unsigned int maxsize = 0;
    for (int i = 0; i != rozmiarDanych; ++i)
    {
        int sum = to_string(i).size();
        auto iterator = listaNastepnikow[i].cbegin();
        while (iterator != listaNastepnikow[i].cend())
            sum += to_string(*(iterator++)).size() + 2;
        if (maxsize <sum) maxsize = sum;
    }
    for (int i = 0; i != rozmiarDanych; ++i)
    {
        auto iterator = listaNastepnikow[i].cbegin();
        cout << i;
        if (listaNastepnikow[i].size()) cout << "->";
        while (iterator != listaNastepnikow[i].cend())
        {
            cout << *(iterator++);
            if (iterator != listaNastepnikow[i].cend()) cout << "->";
        }
        cout << endl;
    }
    cout << endl;
}
