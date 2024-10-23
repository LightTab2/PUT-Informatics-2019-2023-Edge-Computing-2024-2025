#pragma once

void DFSsort_macierzSasiedztwa(short int** macierz, short int rozmiar = rozmiarDanych) 
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
    int i = 0;                                          // WARTOŚĆ POMOCNICZA
    while (brakcyklu) 
    {
        if (stos_head == rozmiar)                       // WARUNEK ZAKOŃCZENIA
            break;

        for (i = 0; i < rozmiar; i++)                   // ZNALEZIENIE NOWEGO WIERZCHOŁKA
        {
            if (kolory[i] == 0) {
                pos = i;
                sciezka[0] = i;
                dlugosc_sciezki = 0;
                break;
            }
        }

        while (brakcyklu) {
            kolory[pos] = 1;                            // ODWIEDZENIE WIERZCHOŁKA POKOLOROWANEGO NA SZARO
            for (i = 0; i < rozmiar; i++)               // WYBRANIE NASTĘPNIKA WIERZCHOŁKA
            {
                if (macierz[pos][i] == 1) {
                    if (kolory[i] == 0) {               // ZNALEZIONO BIAŁY NASTĘPNIK
                        pos = i;
                        dlugosc_sciezki++;
                        sciezka[dlugosc_sciezki] = pos;
                        break;
                    }
                    else if (kolory[i] == 1) {          // WARUNEK NA CYKL
                        brakcyklu = false;
                        i = -2;
                        break;
                    }
                }
            }
            if (i == rozmiar)               // NIE ZNALEZIONO NASTĘPNIKA --- COFANIE SIĘ i DODANIE WIERZCHOŁKA NA STOS (KOLOR CZARNY)
            {
                kolory[pos] = 2;            // KOLOR CZARNY
                stos[stos_head] = pos;
                stos_head++;
                if (dlugosc_sciezki > 0)    // COFNIĘCIE SIĘ W SCIEŻCE
                {
                    dlugosc_sciezki--;
                    pos = sciezka[dlugosc_sciezki];
                }
                else break;                // COFNIĘCIE SIĘ DO POCZĄTKU SCIEŻKI --- POWRÓT DO POCZĄTKU ALGORYTMU
            }
        }

    }

    if (!test) 
    {
        cout << "Sortowanie Topologiczne (Macierz Sasiedztwa DFS):" << endl;
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

bool indeg_macierzSasiedztwa(short int** macierz, short int j, vector<short int> &wierzcholki, short int liczbawierzcholkow = rozmiarDanych)
{
    for (int i : wierzcholki)
    {
        if (macierz[j][i] == -1)
            return 1;
    }
    return 0;
}

bool DELsort_macierzSasiedztwa(short int** graf, short int liczbawierzcholkow = rozmiarDanych)
{
    vector <short int> wierzcholki;
    vector <short int> posortowane;
    for (int i = 0; i < liczbawierzcholkow; i++)
        wierzcholki.push_back(i);
    int n;

    for (int i = 0; i < liczbawierzcholkow; i++)
    {
        n = 0;
        for (int j : wierzcholki)
        {
            if (indeg_macierzSasiedztwa(graf, j, wierzcholki, liczbawierzcholkow) == 0)		
            //indeg() trzeba napisac rozne dla kazdej postaci grafu
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
            return 1; // zwraca error dla cyklu cykl
        }
        else
            wierzcholki.erase(wierzcholki.begin() + n);
    }

    if (!test) 
    {
        cout << "Sortowanie Topologiczne (Macierz Sasiedztwa DEL):" << endl;
        for (int i = 0; i < liczbawierzcholkow; i++)
            cout << (int)posortowane[i] << " ";
        cout << endl;
    }
    return 0;
}

