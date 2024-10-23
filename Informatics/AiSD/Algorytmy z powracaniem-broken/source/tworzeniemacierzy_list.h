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
    if (!macierz)
        return nullptr;
    for (int i = 0 ; i < rozmiar ; i++)
        delete[] macierz[i];
    delete[] macierz;
    return nullptr;
}

vector<short int>* usunListeNastepnikow(vector<short int> *lista = listaNastepnikow, short int rozmiar = rozmiarDanych)
{
    if (!lista)
        return nullptr;
    for (unsigned i = 0; i != rozmiar; ++i)
        lista[i].clear();
    delete[] lista;
    return nullptr;
}