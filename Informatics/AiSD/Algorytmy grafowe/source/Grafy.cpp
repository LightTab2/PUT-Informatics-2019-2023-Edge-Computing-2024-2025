#include <cstring>
#include "ZmienneGlobalne.h"
#include "tworzeniemacierzy_list.h"
#include "macierzSasiedztwa.h"
#include "listanastepnikow.h"
#include "macierzGrafu.h"
#include "Wyswietlanie.h"
#include "DFS.h"

using namespace std;

bool wczytajDane();

int main(int argc, char* args[])
{
    cout << fixed; //Sposób formatowania wyświetlanych liczb
    for (int x = 0; x != argc; ++x) //Ustawienie przełączników
    {
        if (!strcmp(args[x], "-test")) test = true;
        else if (!strcmp(args[x], "-file")) file = true;
    }

    //--------------LOGIKA PROGRAMU----------------
    unsigned long long n = 0;
    if (!wczytajDane())
    {
        cout << "Znaleziono nieprawidlowe dane";
        system("pause");
        return 0;
    }
    //test = true;
    if (!test) cout << "\aUtworzono grafy" << endl;
    if (!test) 
    {
        cout << endl << "Macierz sasiedztwa: " << endl;
        rysujMacierz(macierzSasiedztwa);
    }

    chrono::time_point<chrono::high_resolution_clock> start = chrono::high_resolution_clock::now();
    DELsort_macierzSasiedztwa(macierzSasiedztwa);
    auto duration = chrono::duration_cast<chrono::microseconds>(chrono::high_resolution_clock::now() - start);
    if (test) cout << duration.count() << endl;

    start = chrono::high_resolution_clock::now();
    DFSsort_macierzSasiedztwa(macierzSasiedztwa);
    duration = chrono::duration_cast<chrono::microseconds>(chrono::high_resolution_clock::now() - start);
    if (test) cout << duration.count() << endl;

    if (!test) 
    {
        cout << endl << "Lista nastepnikow: " << endl;
        rysujNastepnikow();
    }
    start = chrono::high_resolution_clock::now();
    DELsort_listaNastepnikow(listaNastepnikow);
    duration = chrono::duration_cast<chrono::microseconds>(chrono::high_resolution_clock::now() - start);
    if (test) cout << duration.count() << endl;

    start = chrono::high_resolution_clock::now();
    DFSsort_listaNastepnikow(listaNastepnikow);
    duration = chrono::duration_cast<chrono::microseconds>(chrono::high_resolution_clock::now() - start);
    if (test) cout << duration.count() << endl;

    if (!test)
    {
        cout << endl << "Macierz grafu: " << endl;
        rysujMacierz(macierzGrafu, true);
    }

    start = chrono::high_resolution_clock::now();
    DELsort_macierzGrafu(macierzGrafu);
    duration = chrono::duration_cast<chrono::microseconds>(chrono::high_resolution_clock::now() - start);
    if (test) cout << duration.count() << endl;

    start = chrono::high_resolution_clock::now();
    DFSsort_macierzGrafu(macierzGrafu);
    duration = chrono::duration_cast<chrono::microseconds>(chrono::high_resolution_clock::now() - start);
    if (test) cout << duration.count() << endl;

    if (!test) cout << "\a" << endl;
    //--------------KONIEC PROGRAMU----------------
    macierzSasiedztwa = usunmacierzSasiedztwa();
    listaNastepnikow = usunListeNastepnikow();
    macierzGrafu = usunmacierzGrafu();
    if (!test) system("pause");
    return 0;
}

//Sprawdza czy dane są liczbami oraz czy zawierają się w przedziale <min; limit>
bool poprawnoscDanych(stringstream& readStream, unsigned int& n, unsigned long long limit = static_cast<unsigned long long>(-1), unsigned long long min = 0ull)
{
    string maxVal = to_string(limit);
    string minVal = to_string(min);
    n = 0u;
    string word;
    bool bDigit = true;
    while (bDigit && readStream >> word)
    {
        if (word.size() > maxVal.size()) bDigit = false;
        else if (word.size() < minVal.size()) bDigit = false;
        else if (word.size() == minVal.size() && word < minVal) bDigit = false;
        else if (word.size() == maxVal.size() && word > maxVal) bDigit = false;
        else
            for (short int c : word)
                if (!isdigit(c))
                    bDigit = false;
        if (n++ > limit) bDigit = false;
    }
    if (n == 0) bDigit = false;
    readStream.clear();
    readStream.seekg(0, ios::beg);
    return bDigit;
}

//Error zostanie wyswietlony, gdy uzytkownik wprowadzi niepoprawne dane. Parametr &n zostanie ustawiony na liczbę równej liczności wczytanych elementów. 
//min i limit zostaną przekazane do poprawność danych
string wprowadzDane(string error, unsigned int& n, unsigned long long limit = static_cast<unsigned long long>(-1), unsigned long long min = 0ull)
{
    string wybor = "", word;
    bool bDigit = true;
    while (wybor.size() == 0 || !bDigit)
    {
        if (!bDigit) cout << error << endl;
        getline(cin, wybor);
        stringstream readStream(wybor);
        bDigit = poprawnoscDanych(readStream, n, limit, min);
    }
    return wybor;
}

//Kod tutaj jest brzydki jak zawsze, ale to tylko interfejs użytkownika, więc treść nie bardzo będąca obiektem rozważań
bool wczytajDane()
{
    string daneWejscioweRaw = "";
    if (file)
    {
        getline(cin, daneWejscioweRaw);

        if (!daneWejscioweRaw.size())
        {
            cout << "Nie udalo sie wczytac danych";    //plik jest pusty albo źle sformatowany
            return false;
        }
        stringstream readStream = stringstream(daneWejscioweRaw);
        unsigned int n = 0;
        if (!poprawnoscDanych(readStream, n))
            return false;
        string word;
        readStream >> word;
        rozmiarDanych = stoul(word);
        readStream >> word;
        liczbaKrawedzi = stoul(word);
        macierzSasiedztwa = new short int* [rozmiarDanych];
        odwiedzone = new bool[rozmiarDanych];
        listaNastepnikow = new vector<short int>[rozmiarDanych];
        for (unsigned i = 0; i != rozmiarDanych; ++i)
        {
            odwiedzone[i] = false;
            macierzSasiedztwa[i] = new short int[rozmiarDanych];
            for (unsigned j = 0; j != rozmiarDanych; ++j)
                macierzSasiedztwa[i][j] = 0;
        }

        getline(cin, daneWejscioweRaw);
        readStream = stringstream(daneWejscioweRaw);

        for (int i = 0; i != liczbaKrawedzi; ++i)
        {
            readStream >> word;
            unsigned a = stoul(word) - 1;
            readStream >> word;
            unsigned b = stoul(word) - 1;
            if (a >= rozmiarDanych)
            {
                cout << "Niepoprawny wierzcholek \"" + to_string(a) + "\"" << endl;
                return false;
            }
            if (b >= rozmiarDanych)
            {
                cout << "Niepoprawny wierzcholek \"" + to_string(b) + "\"" << endl;
                return false;
            }
            if (a == b)
            {
                cout << "Wykryto petle wlasna \"" + to_string(a) + " " + to_string(b) + "\"" << endl;
                return false;
            }
            if (macierzSasiedztwa[a][b])
            {
                cout << "Stworzenie multigrafu jest niedozwolone" << endl;
                return false;
            }
            if (DFS_lista(a))
            {
                cout << "Wykryto cykl, graf nie moze zawierac cyklu" << endl;
                return false;
            }
            macierzSasiedztwa[b][a] = -1;
            macierzSasiedztwa[a][b] = 1;
            if (!count(listaNastepnikow[a].begin(), listaNastepnikow[a].end(), b)) listaNastepnikow[a].push_back(b);
        }
    }
    else
    {
        string wybor = "";
        cout << "Wybierz sposob wprowadzania danych:\nt - wprowadz dane recznie\ng - wygeneruj dane" << endl;
        while (!wybor.size() || (wybor != "t" && wybor != "g"))
        {
            if (wybor.size()) cout << "Wybierz sposob wprowadzania danych:\nt - wprowadz dane recznie\ng - wygeneruj dane" << endl;
            getline(cin, wybor);
            //transform(wybor.begin(), wybor.end(), wybor.begin(), tolower);
        }
        if (wybor == "t")
        {
            cout << "Podaj dwie liczby naturalne liczbe wierzcholkow i liczbe krawedzi:" << endl;
            unsigned n = 0;
            while (true)
            {
                while (n != 2)
                {
                    if (n != 0) cout << "Nalezy podac dokladnie dwie liczby" << endl;
                    daneWejscioweRaw = wprowadzDane("Wprowadzony ciag jest "
                        "niepoprawny. Wszystkie wartosci musza byc liczbami naturalnymi <= " + to_string(maxU) + ". Sporoboj ponownie", n);
                }

                string word;
                stringstream readStream = stringstream(daneWejscioweRaw);
                readStream >> word;
                rozmiarDanych = stoul(word);
                readStream >> word;
                liczbaKrawedzi = stoul(word);
                if (((rozmiarDanych * (rozmiarDanych - 1)) >> 1) >= liczbaKrawedzi) break;
                else cout << "Maksymalna liczba krawedzi dla tego grafu wynosi " << ((rozmiarDanych * (rozmiarDanych - 1)) >> 1) << ". Sporoboj ponownie" << endl;
                n = 0;
            }
            macierzSasiedztwa = new short int* [rozmiarDanych];
            odwiedzone = new bool[rozmiarDanych];
            listaNastepnikow = new vector<short int>[rozmiarDanych];
            for (unsigned i = 0; i != rozmiarDanych; ++i)
            {
                odwiedzone[i] = false;
                macierzSasiedztwa[i] = new short int[rozmiarDanych];
                for (unsigned j = 0; j != rozmiarDanych; ++j)
                    macierzSasiedztwa[i][j] = 0;
            }
            bool proper = false;
            n = 0;
            while (!proper)
            {
                if (!liczbaKrawedzi) break;
                for (int i = 0; i != rozmiarDanych; ++i)
                {
                    listaNastepnikow[i].clear();
                    for (int j = 0; j != rozmiarDanych; ++j)
                        macierzSasiedztwa[i][j] = 0;
                }
                if (n != 0) cout << "Podano niewlasciwa ilosc krawedzi. Liczba krawedzi musi wynosic " + to_string(liczbaKrawedzi) << "." << endl;
                cout << "\nPodaj krawedzie: " << endl;
                daneWejscioweRaw = wprowadzDane("Wprowadzony ciag jest "
                    "niepoprawny. Wszystkie wartosci musza byc liczbami naturalnymi <= " + to_string(maxU) +
                    " i liczba krawedzi musi wynosic " + to_string(liczbaKrawedzi), n);
                if (n != (liczbaKrawedzi << 1)) continue;
                proper = true;
                string word;
                stringstream  readStream = stringstream(daneWejscioweRaw);

                for (int i = 0; i != liczbaKrawedzi; ++i)   //tutaj są wczytywane krawędzie
                {
                    readStream >> word;
                    unsigned a = stoul(word) - 1;
                    readStream >> word;
                    unsigned b = stoul(word) - 1;
                    if (a >= rozmiarDanych)
                    {
                        cout << "Niepoprawny wierzcholek \"" + to_string(a) + "\"" << endl;
                        proper = false;
                        n = 0;
                        break;
                    }
                    if (b >= rozmiarDanych)
                    {
                        cout << "Niepoprawny wierzcholek \"" + to_string(b) + "\"" << endl;
                        proper = false;
                        n = 0;
                        break;
                    }
                    if (a == b)
                    {
                        cout << "Wykryto petle wlasna \"" + to_string(a) + " " + to_string(b) + "\"" << endl;
                        proper = false;
                        n = 0;
                        break;
                    }
                    if (macierzSasiedztwa[a][b])
                    {
                        cout << "Stworzenie multigrafu jest niedozwolone" << endl;
                        proper = false;
                        n = 0;
                        break;
                    }
                    macierzSasiedztwa[b][a] = -1;
                    macierzSasiedztwa[a][b] = 1;
                    listaNastepnikow[a].push_back(b);
                    if (DFS_lista(a))
                    {
                        cout << "Wykryto cykl, graf nie moze zawierac cyklu" << endl;
                        proper = false;
                        n = 0;
                        break;
                    }
                }
            }
            cout << endl;
        }
        else if (wybor == "g")
        {
            mt19937 mt_rand(time(0));
            cout << "Generator upewnia sie, ze wygenerowany graf jest DAG.\nPodaj liczbe wierzcholkow (n >= 0 <= " + to_string(10000u) + "):" << endl;
            bool bDigit = true;
            unsigned int n = 0;
            while (true)
            {
                daneWejscioweRaw = wprowadzDane("Sproboj ponownie: ", n, 10000u);
                if (n != 1) cout << "Musisz podac dokladnie jedna liczbe. Sproboj ponownie: ";
                    else break;
            }
            string word;
            stringstream readStream = stringstream(daneWejscioweRaw);
            readStream >> word;
            rozmiarDanych = stoul(word);
            macierzSasiedztwa = new short int* [rozmiarDanych];
            odwiedzone = new bool[rozmiarDanych];
            listaNastepnikow = new vector<short int>[rozmiarDanych];
            for (unsigned i = 0; i != rozmiarDanych; ++i)
            {
                odwiedzone[i] = false;
                macierzSasiedztwa[i] = new short int[rozmiarDanych];
                for (unsigned j = 0; j != rozmiarDanych; ++j)
                    macierzSasiedztwa[i][j] = 0;
            }
            cout << "Podaj liczbe krawedzi (maksymalne nasycenie wynosi 50%) (0 >= n <= " + to_string(rozmiarDanych * (rozmiarDanych - 1) >> 2) + "):" << endl;

            daneWejscioweRaw = wprowadzDane("Sproboj ponownie: ", n, rozmiarDanych * (rozmiarDanych - 1) >> 2);
            readStream = stringstream(daneWejscioweRaw);
            readStream >> word;
            liczbaKrawedzi = stoul(word);
            
            if (!liczbaKrawedzi) goto end;

            vector<pair<unsigned, unsigned>> mozliwe;
            for (unsigned i = 0; i != rozmiarDanych >> 1; ++i)
                for (unsigned j = rozmiarDanych >> 1; j != rozmiarDanych; ++j)
                    if (i != j) mozliwe.push_back(pair<unsigned, unsigned>(i, j));

            for (int i = 0, end = liczbaKrawedzi - rozmiarDanych + 2; i < end; ++i)  //tworzy graf dwudzielny
            {
                uniform_int_distribution<unsigned> dane_rand(0u, mozliwe.size() - 1u);
                unsigned index = dane_rand(mt_rand);
                pair<unsigned, unsigned> krawedz = mozliwe[index];
                mozliwe.erase(mozliwe.begin() + index);
                auto todel = find(mozliwe.begin(), mozliwe.end(), pair<unsigned, unsigned>(krawedz.second, krawedz.first));
                if (todel != mozliwe.end()) mozliwe.erase(todel);

                listaNastepnikow[krawedz.first].push_back(krawedz.second);
                macierzSasiedztwa[krawedz.second][krawedz.first] = -1;
                macierzSasiedztwa[krawedz.first][krawedz.second] = 1;
            }
            //polacz graf
            vector<unsigned> dostepne;
            for (int i = 0, end = min(liczbaKrawedzi + 1, rozmiarDanych >> 1); i < end; ++i) dostepne.push_back(i);
            unsigned index = 0;
            {
                uniform_int_distribution<unsigned> dane_rand(0u, dostepne.size() - 1u);
                index = dane_rand(mt_rand);
            }
            unsigned first = dostepne[index], second = 0;
            dostepne.erase(dostepne.begin() + index);

            while (dostepne.size())
            {
                uniform_int_distribution<unsigned> dane_rand(0u, dostepne.size() - 1u);
                index = dane_rand(mt_rand);
                second = dostepne[index];
                dostepne.erase(dostepne.begin() + index);

                listaNastepnikow[first].push_back(second);
                macierzSasiedztwa[second][first] = -1;
                macierzSasiedztwa[first][second] = 1;
                first = second;
            }

            dostepne.clear();
            for (int i = rozmiarDanych >> 1, end = min(liczbaKrawedzi + 2, rozmiarDanych); i < end; ++i) dostepne.push_back(i);

            {
                uniform_int_distribution<unsigned> dane_rand(0u, dostepne.size() - 1u);
                index = dane_rand(mt_rand);
            }
            first = 0, second = 0;
            if (dostepne.size())
            {
                first = dostepne[index];
                dostepne.erase(dostepne.begin() + index);
            }

            while (dostepne.size())
            {
                uniform_int_distribution<unsigned> dane_rand(0u, dostepne.size() - 1u);
                index = dane_rand(mt_rand);
                second = dostepne[index];
                dostepne.erase(dostepne.begin() + index);

                listaNastepnikow[first].push_back(second);
                macierzSasiedztwa[second][first] = -1;
                macierzSasiedztwa[first][second] = 1;
                first = second;
            }
        }
    }
    end: macierzGrafu = stworz_macierzGrafu(macierzSasiedztwa);
    return true;
}
