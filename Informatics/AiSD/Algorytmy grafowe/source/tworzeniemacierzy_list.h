#pragma once

short int** iniciuj_macierzSasiedztwa (){
    short int **macierzSasiedztwa = new short int*[rozmiarDanych];
    for (int i = 0; i != rozmiarDanych; ++i)
    {
        macierzSasiedztwa[i] = new short int[rozmiarDanych];
        for (int j = 0; j != rozmiarDanych; ++j)
            macierzSasiedztwa[i][j] = 0;
    }
    return macierzSasiedztwa;
}

void wypelnij_macierziliste(string daneWejscioweRaw,  bool* proper, unsigned int * n ){
    stringstream readStream = stringstream(daneWejscioweRaw);
    string word;
    for (int i = 0; i != liczbaKrawedzi; ++i)   //tutaj są wczytywane krawędzie
    {
        readStream >> word;
        unsigned a = stoul(word);
        readStream >> word;
        unsigned b = stoul(word);
        if (a >= rozmiarDanych)
        {
            cout << "Niepoprawny wierzcholek \"" + to_string(a) + "\"" << endl;
            *proper = false;
            *n = 0;
            break;
        }
        if (b >= rozmiarDanych)
        {
            cout << "Niepoprawny wierzcholek \"" + to_string(b) + "\"" << endl;
            *proper = false;
            *n = 0;
            break;
        }
        if (a == b)
        {
            cout << "Powstala petla wlasna \"" + to_string(a) + " " + to_string(b) + "\"" << endl;
            *proper = false;
            *n = 0;
            break;
        }
        macierzSasiedztwa[a][b] = 1;
        macierzSasiedztwa[b][a] = -1;
        if (!count(listaNastepnikow[a].begin(), listaNastepnikow[a].end(), b)) listaNastepnikow[a].push_back(b);
    }
}

short int ** usunmacierzSasiedztwa ( short int** macierz = macierzSasiedztwa, short int rozmiar = rozmiarDanych)
{
    for (int i = 0 ; i < rozmiar ; i++)
        delete[] macierz[i];
    delete[] macierz;
    return nullptr;
}

short int** stworz_macierzGrafu(short int** macierzSasiedztwa , short int rozmiar = rozmiarDanych)
{
    short int ** macierzGrafu;
    macierzGrafu = new short int *[rozmiar];
    for (int i = 0; i != rozmiar ; ++i) {
        macierzGrafu[i] = new short int[rozmiar+3];
        for (int j = 0; j != rozmiarDanych + 3; ++j)
            macierzGrafu[i][j] = 0;
    }

    short int ostatni_poprzednik , ostatni_nastepnik , ostatni_nieincydenty;
    short int pierwszy_poprzednik , pierwszy_nastepnik , pierwszy_nieincydenty;
    for (int i = 0; i != rozmiar ; ++i)
    {
        ostatni_poprzednik = -1;
        pierwszy_poprzednik = -1;
        ostatni_nastepnik = -1 ;
        pierwszy_nastepnik = -1 ;
        ostatni_nieincydenty = -1;
        pierwszy_nieincydenty = -1;
        for ( int j = rozmiar-1 ; j >= 0 ; j--)
        {
            switch (macierzSasiedztwa[i][j]){
                case -1:                                                   // POPRZEDNIK
                    if(ostatni_poprzednik == -1) ostatni_poprzednik = j;
                    macierzGrafu[i][j] = ostatni_poprzednik + rozmiar;
                    pierwszy_poprzednik = j;
                    break;
                case 0:                                                    // NIEINCYDENTY
                    if(ostatni_nieincydenty == -1) ostatni_nieincydenty = j;
                    macierzGrafu[i][j] = -ostatni_nieincydenty;
                    pierwszy_nieincydenty = j;
                    break;
                case 1:                                                    // NASTEPNIK
                    if(ostatni_nastepnik == -1) ostatni_nastepnik = j;
                    macierzGrafu[i][j] = ostatni_nastepnik;
                    pierwszy_nastepnik = j;
                    break;
            }
        }
        macierzGrafu[i][rozmiar] = pierwszy_nastepnik;
        macierzGrafu[i][rozmiar + 1] = pierwszy_poprzednik;
        macierzGrafu[i][rozmiar + 2] = pierwszy_nieincydenty;
    }
    return macierzGrafu;
}

short int ** usunmacierzGrafu (short int ** macierz = macierzGrafu, short int rozmiar = rozmiarDanych )
{
    for (int i = 0; i != rozmiar; ++i)
        delete[] macierz[i];
    delete[] macierz;
    return nullptr;
}

vector<short int>* usunListeNastepnikow(vector<short int> *lista = listaNastepnikow, short int rozmiar = rozmiarDanych)
{
    for (unsigned i = 0; i != rozmiar; ++i)
        lista[i].clear();
    delete[] lista;
    return nullptr;
}