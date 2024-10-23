#pragma once
short int *wierzcholki;
short int wierzcholki_head;
short int *euler;
short int euler_head;

bool most_macierz_nieskierowana(short int wierzcholek , short int  wierzcholek_euler ,short int**& macierz = macierzSasiedztwa , short int rozmiar = rozmiarDanych)
{
    macierz[wierzcholek_euler][wierzcholek]=0;
    macierz[wierzcholek][wierzcholek_euler]=0;
    bool * odwiedzone = new bool [rozmiar];
    short int* sciezka = new short int [rozmiar];
    short int sciezka_head = 0;
    sciezka[0] = wierzcholek;
    for(int i = 0 ; i < rozmiar ; i++)
        odwiedzone[i] = 0;
    odwiedzone[wierzcholek]=1;
    for ( int i = 0 ; i < rozmiar ; i++)
    {
        if (macierz[sciezka[sciezka_head]][i]==1) {
            if ( !odwiedzone[i] )
            {
                sciezka_head++;
                sciezka[sciezka_head]=i;
                odwiedzone[i]=1;
                i = -1;
                if(sciezka[sciezka_head]==wierzcholek_euler)
                {
                    macierz[wierzcholek_euler][wierzcholek]=1;
                    macierz[wierzcholek][wierzcholek_euler]=1;
                    delete[] odwiedzone;
                    delete[] sciezka;
                    return false;
                }
            }
        }
        if (i == rozmiar-1)
        {
            if (sciezka[sciezka_head]!=wierzcholek)
            {
                sciezka_head--;
            }
            else
            {
                int l = 0;
                for (int j = 0; j < rozmiar ; j++)
                {
                    if (odwiedzone[j])
                        l++;
                }
                if (l==rozmiar)
                {
                    macierz[wierzcholek_euler][wierzcholek]=1;
                    macierz[wierzcholek][wierzcholek_euler]=1;
                    delete[] odwiedzone;
                    delete[] sciezka;
                    return false;
                }
                else
                {
                    macierz[wierzcholek_euler][wierzcholek]=1;
                    macierz[wierzcholek][wierzcholek_euler]=1;
                    delete[] odwiedzone;
                    delete[] sciezka;
                    return true;
                }
            }
        }

    }
    macierz[wierzcholek_euler][wierzcholek]=1;
    macierz[wierzcholek][wierzcholek_euler]=1;
    delete[] odwiedzone;
    delete[] sciezka;
    return false;
}
bool stopien_parzystosc(short int**& macierz = macierzSasiedztwa , short int rozmiar = rozmiarDanych)
{
    short int stopien;
    for(int i = 0 ; i < rozmiar ; i++)
    {
        stopien = 0;
        for(int j = 0; j < rozmiar ; j++)
        {
            if(macierz[i][j])
                stopien++;
        }
        if(stopien%2)
            return 0;
    }
    return 1;
}

void Euler_macierz(short int**& macierz, short int rozmiar = rozmiarDanych , short int ilosckrawedzi = liczbaKrawedzi)
{
    bool sprawdzam_mosty = 1;
    for (int i = 0 ; i < rozmiar ; i++)
    {
        if (macierz[wierzcholki[wierzcholki_head]][i]==1)
        {
            if ( !sprawdzam_mosty || !most_macierz_nieskierowana(i,wierzcholki[wierzcholki_head] , macierz , rozmiar) )
            {
            macierz[wierzcholki[wierzcholki_head]][i]=0;
            macierz[i][wierzcholki[wierzcholki_head]]=0;
            wierzcholki_head++;
            wierzcholki[wierzcholki_head]=i;
            Euler_macierz(macierz , rozmiar , ilosckrawedzi);
            }
        }
        if (i == rozmiar-1&&sprawdzam_mosty)
        {
            sprawdzam_mosty=0;
            i=-1;
        }
    }
    euler_head++;
    euler[euler_head]=wierzcholki[wierzcholki_head];
    wierzcholki_head--;
}

void Euler_macierzstart(short int**& macierz = macierzSasiedztwa, short int rozmiar = rozmiarDanych , short int ilosckrawedzi = liczbaKrawedzi)
{
    if(rozmiar > 1 && ilosckrawedzi != 0) {
        wierzcholki = new short int[liczbaKrawedzi + 1];
        euler = new short int[liczbaKrawedzi + 1];
        wierzcholki[0] = 0;
        wierzcholki_head = 0;
        euler_head = -1;
        if (stopien_parzystosc(macierz, rozmiar)) {
            Euler_macierz(macierz, rozmiar, ilosckrawedzi);
            if (ilosckrawedzi == (euler_head)) {
                for (int i = 0; i <= ilosckrawedzi; i++)
                    cout << euler[i] << " ";
                cout << endl;
            }
        }
        delete[] wierzcholki;
        delete[] euler;
    }
}

bool most_lista_skierowana(short int wierzcholek , short int  wierzcholek_euler ,vector<short int> *& nastepniki = listaNastepnikow, short int rozmiar = rozmiarDanych)
{
    bool * odwiedzone = new bool [rozmiar];
    short int* sciezka = new short int [rozmiar];
    short int sciezka_head = 0;
    sciezka[0] = wierzcholek;
    for(int i = 0 ; i < rozmiar ; i++)
        odwiedzone[i] = 0;
    odwiedzone[wierzcholek]=1;
    for ( int i = 0 ; i < nastepniki[sciezka[sciezka_head]].size() ; i++)
    {
            if ( !odwiedzone[nastepniki[sciezka[sciezka_head]][i]] )
            {
                sciezka_head++;
                if (sciezka_head >= rozmiar)
                {
                    delete[] odwiedzone;
                    delete[] sciezka;
                    return false;
                }
                sciezka[sciezka_head]=i;
                odwiedzone[i]=1;
                i = -1;
                if(sciezka[sciezka_head]==wierzcholek_euler)
                {
                    delete[] odwiedzone;
                    delete[] sciezka;
                    return false;
                }
            }
        if (i == rozmiar-1)
        {
            if (sciezka[sciezka_head]!=wierzcholek)
            {
                sciezka_head--;
            }
            else
            {
                int l = 0;
                for (int j = 0; j < rozmiar ; j++)
                {
                    if (odwiedzone[j])
                        l++;
                }
                if (l==rozmiar)
                {
                    delete[] odwiedzone;
                    delete[] sciezka;
                    return false;
                }
                else
                {
                    delete[] odwiedzone;
                    delete[] sciezka;
                    return true;
                }
            }
        }

    }
    delete[] odwiedzone;
    delete[] sciezka;
    return false;
}

bool stopien_lista(vector<short int> *nastepniki = listaNastepnikow , short int rozmiar = rozmiarDanych)
{
    short int *stopien = new short int [rozmiar];
    for (int i = 0 ; i < rozmiar ; i++)
        stopien[i]=0;
    for(int i = 0 ; i < rozmiar ; i++)
    {
        stopien[i]=stopien[i]+nastepniki[i].size();
        for(int j = 0; j < nastepniki[i].size() ; j++)
        {
            stopien[nastepniki[i][j]]--;
        }
    }
    for(int i = 0; i < rozmiar ; i++)
    {
        if(stopien[i]!=0)
        {
            delete[] stopien;
            return 0;
        }
    }
    delete[] stopien;
    return 1;
}

void Euler_lista(vector<short int> *nastepniki = listaNastepnikow, short int rozmiar = rozmiarDanych , short int ilosckrawedzi = liczbaKrawedzi)
{
    bool sprawdzam_mosty = 1;
    for ( int i = 0 ; i < nastepniki[wierzcholki[wierzcholki_head]].size() ; i++)
    {
        if ( !sprawdzam_mosty || !most_lista_skierowana(i,wierzcholki[wierzcholki_head] , nastepniki , rozmiar) ) {
            wierzcholki_head++;
            wierzcholki[wierzcholki_head] = nastepniki[wierzcholki[wierzcholki_head - 1]].front();
            nastepniki[wierzcholki[wierzcholki_head - 1]].erase(nastepniki[wierzcholki[wierzcholki_head - 1]].begin());
            Euler_lista(nastepniki, rozmiar, ilosckrawedzi);
        }
        if (i == rozmiar-1&&sprawdzam_mosty)
        {
            sprawdzam_mosty=0;
            i=-1;
        }
    }
    euler_head++;
    euler[euler_head]=wierzcholki[wierzcholki_head];
    wierzcholki_head--;
}

void Euler_listastart(vector<short int> *nastepniki = listaNastepnikow, short int rozmiar = rozmiarDanych , short int ilosckrawedzi = liczbaKrawedzi)
{
    if(rozmiar > 1 && ilosckrawedzi != 0) {
        wierzcholki = new short int[liczbaKrawedzi + 1];
        euler = new short int[liczbaKrawedzi + 1];
        wierzcholki[0] = 0;
        wierzcholki_head = 0;
        euler_head = -1;
        if (stopien_lista(nastepniki, rozmiar)) {
            Euler_lista(nastepniki, rozmiar, ilosckrawedzi);
            if (ilosckrawedzi == (euler_head)) {
                for (int i = ilosckrawedzi; i >= 0; i--)
                    cout << euler[i] << " ";
                cout << endl;
            }
        }
        delete[] wierzcholki;
        delete[] euler;
    }
}


void znajdzCyklEulera()
{
    if (mode) Euler_listastart();
    else Euler_macierzstart();
}