#pragma once
#include <iostream>
#include <stdio.h>
using namespace std;

unsigned long long*selection_sort( struct Przedmiot  *przetwarzane_przedmioty , unsigned long long ilosc)
{
    unsigned long long*sorted_tab = new unsigned long long[ilosc];
    double *wspolczynnik = new double[ilosc];
    unsigned long long temp;
    unsigned long long temp_wartosc;
    for (unsigned long long i = 0 ; i <ilosc ; i ++)
    {
        sorted_tab[i]=i;
        if(przetwarzane_przedmioty[i].waga)
            wspolczynnik[i]=przetwarzane_przedmioty[i].cena/(double)przetwarzane_przedmioty[i].waga;
        else
            wspolczynnik[i]=numeric_limits<double>::max();
    }
    for (unsigned long long i = 0 ; i < ilosc ; i ++)
    {
        temp = i;
        for (unsigned long long j = i+1 ; j < ilosc ; j ++)
        {
            if (wspolczynnik[sorted_tab[j]]>wspolczynnik[sorted_tab[temp]])
            {
                temp = j;
            }
        }
        temp_wartosc = sorted_tab[i];
        sorted_tab[i]=sorted_tab[temp];
        sorted_tab[temp]=temp_wartosc;
    }
    delete[] wspolczynnik;
    return sorted_tab;
}

void AZ (  struct Przedmiot *przetwarzane_przedmioty = przedmioty , unsigned long long ilosc = liczba_przedmiotow , unsigned long long miejsce = pojemnosc_plecaka  )
{
    if (!miejsce)
    {
        if(!test)
        cout << "AZ: Brak miejsca na przedmioty" <<endl;
        return;
    }
    unsigned long long waga = 0;
    unsigned long long wartosc = 0;
    unsigned long long* plecak = new unsigned long long[ilosc];
    unsigned long long plecak_head = 0;
    unsigned long long* sorted = selection_sort( przetwarzane_przedmioty , ilosc);
    for (unsigned long long i = 0 ; i < ilosc; i++)
    {
        if (waga +przetwarzane_przedmioty[sorted[i]].waga <= miejsce)
        {
            plecak[plecak_head]=sorted[i];
            plecak_head++;
            waga = waga + przetwarzane_przedmioty[sorted[i]].waga;
            wartosc = wartosc + przetwarzane_przedmioty[sorted[i]].cena;
        }
    }
    if (!test)
    {
        cout << "AZ: Obiekty w plecaku (id):  ";
        if (plecak_head) {
            for (unsigned long long i = 0; i < plecak_head; i++)
                cout << plecak[i]+1 << " ";
            cout << "\nWartosc przedmiotow lacznie: "  << wartosc << "\n";
            cout << "Waga przedmiotow lacznie: "  << waga << "\n";
        }else
            cout << "AZ: Brak miejsca na przedmioty." <<endl;
    }
    delete[] plecak;
    delete[] sorted;
    return;
}
