#pragma once

void DFSsort_listaNastepnikow(vector<short int>* const &tab, short int rozmiar = rozmiarDanych) {
    short int* stos = new short int[rozmiar];           // STOS CZARNYCH WIERZCHOŁKÓW
    short int* sciezka = new short int[rozmiar];        // DROGA DO COFNIĘCIA SIĘ
    short int dlugosc_sciezki = 0;                      // PRZEBYTA DROGA
    short int stos_head = 0;                            // ILOŚĆ CZARNYCH WIERZCHOŁKÓW
    short int* kolory = new short int[rozmiar];         // POKOLOROWANIE NA BIAŁO
    for (int i = 0; i < rozmiar; i++)
        kolory[i] = 0;
    int pos;                                            // POZYCJA NA GRAFIE
    bool brakcyklu = true;
    int i = 0;
    bool bialy_nastepnik = false;
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
            bialy_nastepnik = false;
            for (i = 0; i < tab[pos].size(); i++)       // WYBRANIE NASTĘPNIKA WIERZCHOŁKA
            {
                if (tab[pos].size() != 0) {
                    if (kolory[tab[pos][i]] == 0) {
                        pos = tab[pos][i];
                        dlugosc_sciezki++;
                        sciezka[dlugosc_sciezki] = pos;
                        bialy_nastepnik = true;
                        break;
                    }
                    else if (kolory[tab[pos][i]] == 1)  // WARUNEK NA CYKL
                    {  
                        brakcyklu = false;
                        bialy_nastepnik = true;
                        break;
                    }
                }
            }

            if (!bialy_nastepnik)                       // NIE ZNALEZIONO NASTĘPNIKA -- COFANIE SIĘ i DODANIE WIERZCHOŁKA NA STOS ( KOLOR CZARNY)
            {
                kolory[pos] = 2;                        // KOLOR CZARNY
                stos[stos_head] = pos;
                stos_head++;
                if (dlugosc_sciezki > 0)                // COFNIECIĘ SIĘ W SCIEŻCE
                {
                    dlugosc_sciezki--;
                    pos = sciezka[dlugosc_sciezki];
                }
                else  break;                            // COFNIĘCIE SIĘ DO POCZĄTKU SCIEŻKI --- POWRÓT DO POCZĄTKU ALGORYTMU
            }
        }
    }

    if (!test) 
    {
        cout << "Sortowanie Topologiczne (Lista Nastepnikow DFS):" << endl;
        if (brakcyklu) {
            for (i = rozmiar - 1; i >= 0; i--)
                cout << (int)stos[i] << " ";
            cout << endl;
        }
        else
            cout << endl << " Znaleziono cykl, nie mozna wykonac sortowania" << endl;
    }
    delete[] stos;
    delete[] sciezka;
    delete[] kolory;
}

bool DELsort_listaNastepnikow(vector<short int>* const &lista, short int liczbaWierzcholkow = rozmiarDanych)
{
    unsigned int* indeg = new unsigned int[liczbaWierzcholkow];
    for (unsigned int i = 0; i != liczbaWierzcholkow; ++i)
        indeg[i] = 0;

    for (unsigned int i = 0; i != liczbaWierzcholkow; ++i)
        for (auto it = lista[i].cbegin(); it != lista[i].cend(); ++it)
            ++indeg[*it];

    vector <short int> wierzcholki;
    vector <short int> posortowane;
    for (unsigned int i = 0; i != liczbaWierzcholkow; ++i)
        wierzcholki.push_back(i);
    for (int i = 0; i < liczbaWierzcholkow; i++)
    {
        unsigned n = 0;
        for (int j : wierzcholki)
        {
            if (indeg[j] == 0)
            {
                posortowane.push_back(j);
                for (auto it = lista[j].cbegin(); it != lista[j].cend(); ++it)
                    --indeg[*it];
                break;
            }
            ++n;
        }
        if (n == wierzcholki.size()) //nie ma wierzcholka o indeg = 0
        {
            cout << "Znaleziono cykl, nie mozna wykonac sortowania" << endl;
            delete[] indeg;
            return true; // zwraca error dla cyklu
        }
        else
            wierzcholki.erase(wierzcholki.begin() + n);
    }
    if (!test)
    {
        cout << "Sortowanie Topologiczne (Lista Nastepnikow DEL):" << endl;
        for (int i = 0; i < liczbaWierzcholkow; i++)
            cout << (int)posortowane[i] << " ";
        cout << endl;
    }
    delete[] indeg;
    return false;
}
