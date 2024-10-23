#pragma once
#include <math.h>
#include <chrono>
#include <ctime>
#include <cstring>
#include <iostream>
#include <fstream>
#include <sstream>
#include <random>
#include <algorithm>

using namespace std;

void maxKopiec(unsigned long long* tablica, int rodzic, int end)
{
    int dziecko1 = rodzic << 1, dziecko2 = dziecko1 + 1;
    if (dziecko1 > end || rodzic == 0) return;

    if (dziecko2 <= end && tablica[dziecko2] < tablica[dziecko1])
    {
        if (tablica[rodzic] > tablica[dziecko2])
        {
            unsigned long long copy = tablica[rodzic];
            tablica[rodzic] = tablica[dziecko2];
            tablica[dziecko2] = copy;
            maxKopiec(tablica, dziecko2, end);
        }
    }
    else if (tablica[rodzic] > tablica[dziecko1])
    {
        unsigned long long copy = tablica[rodzic];
        tablica[rodzic] = tablica[dziecko1];
        tablica[dziecko1] = copy;
        maxKopiec(tablica, dziecko1, end);
    }
}

void heapSort(unsigned long long* tablica, int end)
{
    //utwórz kopiec
    for (int i = end; i != 0; --i)
        maxKopiec(tablica, i, end);
    for ( int i = 1 ; i != end;)
    {
        unsigned long long copy = tablica[end];
        tablica[end] = tablica[1];
        tablica[1] = copy;
        i++;
        maxKopiec(tablica, 1, end);
    }
}