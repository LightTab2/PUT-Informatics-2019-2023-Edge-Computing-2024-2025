#pragma once

void DFSsort_macierzGrafu(short int** &macierz, short int rozmiar = rozmiarDanych)
{
    short int* stos = new short int[rozmiar];           // STOS CZARNYCH WIERZCHOŁKÓW
    short int* sciezka = new short int[rozmiar];        // DROGA DO COFNIĘCIA SIĘ
    short int dlugosc_sciezki = 0;                      // PRZEBYTA DROGA
    short int stos_head = 0;                            // ILOŚĆ CZARNYCH WIERZCHOŁKÓW

    short int* kolory = new short int[rozmiar];         // POKOLOROWANIE NA BIAŁO
    for (int i = 0; i < rozmiar; i++) 
        kolory[i] = 0;
    int pos;                                            // POZYCJA NA GRAFIE
    bool brakcyklu = true;
    short int ostatni_nastepnik;
    int i = 0;                                          // WARTOŚĆ POMOCNICZA
    while (brakcyklu)
    {
        if (stos_head == rozmiar)                       // WARUNEK ZAKOŃCZENIA
            break;

        for (i = 0; i < rozmiar; i++)                   // ZNALEZIENIE NOWEGO WIERZCHOŁKA
        {
            if (kolory[i] == 0)
            {
                pos = i;
                sciezka[0] = i;
                dlugosc_sciezki = 0;
                break;
            }
        }

        while (brakcyklu)
        {
            kolory[pos] = 1;                                            // ODWIEDZENIE WIERZCHOŁKA POKOLOROWANEGO NA SZARO
            i = macierz[pos][macierz[pos][rozmiar]] + 1;
            ostatni_nastepnik = macierz[pos][macierz[pos][rozmiar]];
            if (macierz[pos][rozmiar] != -1) {                          // SPRAWDZENIE, CZY MA JAKIEKOLWIEK NASTĘPNIKI
                for (i = macierz[pos][rozmiar]; i <= macierz[pos][macierz[pos][rozmiar]]; i++)         // WYBRANIE NASTĘPNIKA WIERZCHOLKA
                {
                    if (macierz[pos][i] == macierz[pos][macierz[pos][rozmiar]]) {
                        if (kolory[i] == 0)                             // ZNALEZIONO BIAŁY NASTĘPNIK
                        {                                               
                            pos = i;
                            dlugosc_sciezki++;
                            sciezka[dlugosc_sciezki] = pos;
                            break;
                        }
                        else
                            if (kolory[i] == 1)                         // WARUNEK NA CYKL
                            {                                       
                                brakcyklu = false;
                                i = -2;
                                break;
                            }
                    }
                }
            }
            if (i == ostatni_nastepnik + 1)   // NIE ZNALEZIONO NASTĘPNIKA -- COFANIE SIĘ i DODANIE WIERZCHOŁKA NA STOS (KOLOR CZARNY)
            {
                kolory[pos] = 2;                    // KOLOR CZARNY
                stos[stos_head] = pos;
                stos_head++;
                if (dlugosc_sciezki > 0)            // COFNIĘCIE SIE W SCIEŻCE
                {
                    dlugosc_sciezki--;
                    pos = sciezka[dlugosc_sciezki];
                }
                else break;                         // COFNIĘCIE SIĘ DO POCZĄTKU SCIEŻKI --- POWRÓT DO POCZĄTKU ALGORYTMU
            }
        }

    }

    if (!test) {
        cout << "Sortowanie Topologiczne (Macierz Grafu DFS):" << endl;
        if (brakcyklu) {
            for (i = rozmiar - 1; i >= 0; i--)
                cout << (int)stos[i] << " ";
            cout << endl;
        }
        else
            cout << endl << "Znaleziono cykl, nie mozna wykonac sortowania" << endl;
    }
    delete[] stos;
    delete[] sciezka;
    delete[] kolory;
}

bool indeg_macierzGrafu(short int** &macierz, short int j, vector<short int> &wierzcholki, int *&pierwsze_poprzedniki , int  ostatni_poprzednik , short int liczbawierzcholkow = rozmiarDanych)
{
    if (pierwsze_poprzedniki[j] == -1 || ostatni_poprzednik < pierwsze_poprzedniki[j] )   // brak poprzednikow || usunieto wszystkie
        return false;
    for (int i = pierwsze_poprzedniki[j] ; i <= ostatni_poprzednik ; i++)
    {
        if (macierz[j][wierzcholki[i]] >= liczbawierzcholkow)
        {
            pierwsze_poprzedniki[j]=i;
            return true;
        }
    }
    return false;
}

bool DELsort_macierzGrafu(short int** &graf, short int liczbawierzcholkow = rozmiarDanych)
{
    vector <short int> wierzcholki;
    vector <short int> posortowane;
    int *pierwsze_poprzedniki = new int [liczbawierzcholkow];
    int *ostatnie_poprzedniki = new int [liczbawierzcholkow];
    for(int i = 0 ; i < liczbawierzcholkow ; i++)
    {
        pierwsze_poprzedniki[i] = graf[i][liczbawierzcholkow+1];
        ostatnie_poprzedniki[i] = graf[i][pierwsze_poprzedniki[i]]-liczbawierzcholkow;
    }

    for (int i = 0; i < liczbawierzcholkow; i++)
        wierzcholki.push_back(i);
    int n;
    for (int i = 0; i < liczbawierzcholkow; i++)
    {
        n = 0;
        for (int j : wierzcholki)
        {
            if (indeg_macierzGrafu(graf, j, wierzcholki, pierwsze_poprzedniki , ostatnie_poprzedniki[j] , liczbawierzcholkow) == 0)
                // indeg() trzeba napisac rozne dla kazdej postaci grafu
                // graf     - w ktorym grafie trzeba sprawdzic
                // j 	    - wierzcholek do sprawdzenia
                // wierzcholki - ktore wierzcholki trzeba brac pod uwage
            {
                posortowane.push_back(j);
                break;
            }
            n++;
        }
        if (n >= wierzcholki.size())
        {
            cout << "Znaleziono cykl, nie mozna wykonac sortowania" << endl;
            delete[] pierwsze_poprzedniki;
            delete[] ostatnie_poprzedniki;
            return true; // zwraca error dla cyklu
        }
        else
        {
            wierzcholki.erase(wierzcholki.begin() + n);
            for(int j = 0 ; j < liczbawierzcholkow ; j++)
            {
                if( n < pierwsze_poprzedniki[j]) pierwsze_poprzedniki[j]--;
                if( n <= ostatnie_poprzedniki[j]) ostatnie_poprzedniki[j]--;
            }
        }
    }
    if (!test)
    {
        cout << "Sortowanie Topologiczne (Macierz Grafu DEL):" << endl;
        for (int i = 0; i < liczbawierzcholkow; i++)
            cout << (int)posortowane[i] << " ";
        cout << endl;
    }
    delete[] pierwsze_poprzedniki;
    delete[] ostatnie_poprzedniki;
    return false;
}