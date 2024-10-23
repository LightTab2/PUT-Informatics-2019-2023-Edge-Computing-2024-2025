#pragma once
#include <iostream>
#include <stdio.h>
using namespace std;

/*rekurencyjny jest wolniejszy
void AW_rek(unsigned long long i, bool *visited, unsigned long long& waga, unsigned long long& cena, unsigned long long &max, bool* solution, Przedmiot* przetwarzane_przedmioty = przedmioty, unsigned long long ilosc = liczba_przedmiotow, unsigned long long miejsce = pojemnosc_plecaka)
{
    if (visited[i] || (waga + przetwarzane_przedmioty[i].waga) > miejsce) return;
    visited[i] = true;
    //Uzwględniamy rozwiązania zawierające przedmiot i
    waga += przetwarzane_przedmioty[i].waga;   
    cena += przetwarzane_przedmioty[i].cena;
    if (cena > max) //znaleziono lepsze rozwiązanie 
    {
        max = cena;
        memcpy(solution, visited, sizeof(bool) * ilosc);
    }
    for (unsigned long long i = 0; i != ilosc; ++i)
        AW_rek(i, visited, waga, cena, max, solution);
    //Przestajemy uwzględniać przedmiot i
    waga -= przetwarzane_przedmioty[i].waga;
    cena -= przetwarzane_przedmioty[i].cena;
    visited[i] = false;
}

void AW(Przedmiot* przetwarzane_przedmioty = przedmioty, unsigned long long ilosc = liczba_przedmiotow, unsigned long long miejsce = pojemnosc_plecaka)
{
    bool* visited = new bool[ilosc], *solution = new bool[ilosc];
    memset(visited, false, sizeof(bool) * ilosc);
    unsigned long long max = 0, cena = 0, waga = 0;
    for (unsigned long long i = 0; i != ilosc; ++i)
        AW_rek(i, visited, waga, cena, max, solution);

    if (!test) 
    {
        unsigned long long liczbaPrzedmiotow = 0;
        for (unsigned long long i = 0; i != ilosc; ++i)
            if (solution[i]) ++liczbaPrzedmiotow;
            
        if (!liczbaPrzedmiotow)
            cout << "AW: Brak miejsca na przedmioty" << endl;
        else
        {
            cout << "AW: Obiekty w plecaku (id):  ";
            unsigned long long suma_wag = 0;
            for (unsigned long long i = 0; i != ilosc; ++i)
            {
                if (solution[i])
                {
                    cout << (i + 1) << " ";
                    suma_wag += przetwarzane_przedmioty[i].waga;
                }
            }
            cout << "\nWartosc przedmiotow lacznie: " << max << "\n";
            cout << "Waga przedmiotow lacznie: " << suma_wag << "\n";
        }   
    }
}
*/

void AW(Przedmiot *przetwarzane_przedmioty = przedmioty, unsigned long long ilosc = liczba_przedmiotow, unsigned long long miejsce = pojemnosc_plecaka)
{
    if (ilosc > 63)
    {
        if (!test)
            cout << "AW: przekroczono maksymalna liczbe przedmiotow dla tego algorytmu rowna 63 (wieksze powoduja blad wykonania)" << endl;
        return;
    }
    if (!miejsce)
    {
        if(!test)
            cout << "AW: Brak miejsca na przedmioty" << endl;
        return;
    }
    unsigned long long *tab_best = new unsigned long long[ilosc];
    unsigned long long tab_best_head = 0;
    unsigned long long wynik_best = 0;
    unsigned long long miejsce_best = 0;
    unsigned long long *tab_temp = new unsigned long long[ilosc];
    unsigned long long tab_temp_head = 0;
    unsigned long long wynik_temp = 0;
    unsigned long long miejsce_temp = 0;
    unsigned long long temp;
    for (unsigned long long i = 0; i <= (unsigned long long)((1ull<<ilosc)-1ull)  ; i++)
    {
        wynik_temp = 0;
        tab_temp_head = 0;
        temp = i;
        miejsce_temp = 0;
        for (unsigned long long j = 0 ; j < ilosc ; j++)
        {
            if (temp > 0 && miejsce != miejsce_temp)
            {
                if ((temp & 1) == 1) {
                    if((miejsce_temp + przetwarzane_przedmioty[j].waga) <= miejsce)
                    {
                        tab_temp[tab_temp_head]=j;
                        tab_temp_head++;
                        wynik_temp= wynik_temp+przetwarzane_przedmioty[j].cena;
                        miejsce_temp = miejsce_temp+przetwarzane_przedmioty[j].waga;
                    }
                    else
                        break;
                }
            }
            else
                break;
            temp>>=1;
        }
        if (wynik_temp>wynik_best)
        {
            wynik_best=wynik_temp;
            miejsce_best=miejsce_temp;
            tab_best_head = tab_temp_head;
            delete[] tab_best;
            tab_best = tab_temp;
            tab_temp = new unsigned long long[ilosc];
        }
    }
    if (!test) {
        cout << "AW: Obiekty w plecaku (id):  ";
        if(tab_best_head) {
            for (unsigned long long i = 0; i < tab_best_head; i++)
                cout << tab_best[i] + 1 << ' ';
            cout << "\nWartosc przedmiotow lacznie: "  << wynik_best << "\n";
            cout << "Waga przedmiotow lacznie: "  << miejsce_best << "\n";
        }else
            cout << "AW: Brak miejsca na przedmioty" <<endl;
    }
    delete[] tab_temp;
    delete[] tab_best;
    return;
}
