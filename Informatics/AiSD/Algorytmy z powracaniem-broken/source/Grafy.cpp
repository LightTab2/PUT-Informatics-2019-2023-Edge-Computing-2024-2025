#include "ZmienneGlobalne.h"
#include "tworzeniemacierzy_list.h"
#include "Wyswietlanie.h"
#include "Hamilton.h"
#include "euler.h"
#include <fstream>
using namespace std;
bool wczytajDane(); 
void wygenerujDane(int nasycenie);

int main(int argc, char* args[])
{
    srand(time(0));
    cout << fixed; //Sposób formatowania wyświetlanych liczb
    for (int x = 0; x != argc; ++x) //Ustawienie przełączników
    {
        if (!strcmp(args[x], "-test")) test = true;
        else if (!strcmp(args[x], "-file")) file = true;
    }
    file = true;
    test = true;
    //--------------LOGIKA PROGRAMU----------------
    unsigned long long n = 0;

    for (int x = 1; x != 10; ++x)
        for (int y = 0; y != 10; ++y)
            for (int z = 0; z != 10; ++z)
            {
                macierzSasiedztwa = usunmacierzSasiedztwa();
                listaNastepnikow = usunListeNastepnikow();
                rozmiarDanych = 20 + z * 20;
                mode = 0;
                {
                    ofstream out("Wyniki/n" + to_string(x) + "_" + to_string(y) + "_" + to_string(z));
                    start = chrono::high_resolution_clock::now();
                    auto duration = chrono::duration_cast<chrono::microseconds>(chrono::high_resolution_clock::now() - start);
                    wygenerujDane(x);
                    start = chrono::high_resolution_clock::now();
                    znajdzCyklHamiltona();
                    duration = chrono::duration_cast<chrono::microseconds>(chrono::high_resolution_clock::now() - start);
                    out << duration.count() << endl;
                    cout << duration.count() << endl;

                    start = chrono::high_resolution_clock::now();
                    znajdzCyklEulera();
                    duration = chrono::duration_cast<chrono::microseconds>(chrono::high_resolution_clock::now() - start);
                    out << duration.count();
                    cout << duration.count() << endl;
                    out.close();
                }
                mode = 1;
                {
                    ofstream out("Wyniki/s" + to_string(x) + "_" + to_string(y) + "_" + to_string(z));
                    start = chrono::high_resolution_clock::now();
                    auto duration = chrono::duration_cast<chrono::microseconds>(chrono::high_resolution_clock::now() - start);
                    wygenerujDane(x);
                    start = chrono::high_resolution_clock::now();
                    znajdzCyklHamiltona();
                    duration = chrono::duration_cast<chrono::microseconds>(chrono::high_resolution_clock::now() - start);
                    out << duration.count() << endl;
                    cout << duration.count() << endl;

                    start = chrono::high_resolution_clock::now();
                    znajdzCyklEulera();
                    duration = chrono::duration_cast<chrono::microseconds>(chrono::high_resolution_clock::now() - start);
                    out << duration.count();
                    cout << duration.count() << endl;
                    out.close();
                }
            }

    system("pause");
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
//min i limit zostaną przekazane do poprawnoscDanych(...)
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
bool wprowadzDane()
{
    string daneWejscioweRaw = "";
    if (!file) cout << "Podaj dwie liczby naturalne liczbe wierzcholkow (> 0) i liczbe krawedzi:" << endl;

    unsigned n = 0;
    while (true)
    {
        while (n != 2)
        {
            if (n != 0) {
                cout << "Nalezy podac dokladnie dwie liczby" << endl;
                if (file) return false;
            }
            if ((daneWejscioweRaw = wprowadzDane("Wprowadzony ciag jest "
                "niepoprawny. Wszystkie wartosci musza byc liczbami naturalnymi <= " + to_string(maxU) + ". Sproboj ponownie:", n, maxU)) == "" && file) return false;
        }
        n = 0;
        string word;
        stringstream readStream = stringstream(daneWejscioweRaw);
        readStream >> word;
        if (!(rozmiarDanych = stoul(word)))
        {
            cout << "Liczba wierzcholkow nie moze sie rownac 0." << endl;
            if (file) return false;
            continue;
        }
        readStream >> word;
        liczbaKrawedzi = stoul(word);
        unsigned max_krawedzi = rozmiarDanych * (rozmiarDanych - 1);
        if (!mode) max_krawedzi >>= 1;
        if (liczbaKrawedzi <= max_krawedzi) break;
        else {
            cout << "Maksymalna liczba krawedzi dla tego grafu wynosi " << max_krawedzi << ". Sproboj ponownie:" << endl;
            if (file) return false;
        }
    }
    macierzSasiedztwa = new short int* [rozmiarDanych];
    listaNastepnikow = new vector<short int>[rozmiarDanych];
    for (unsigned i = 0; i != rozmiarDanych; ++i)
    {
        macierzSasiedztwa[i] = new short int[rozmiarDanych];
        for (unsigned j = 0; j != rozmiarDanych; ++j)
            macierzSasiedztwa[i][j] = 0;
    }
    for (int i = 0; i != liczbaKrawedzi; ++i)   //tutaj są wczytywane krawędzie
    {
        bool bProper = false;
        unsigned a, b;
        while (!bProper)
        {
            if (!file) cout << "\nPodaj " + to_string(i + 1) + ". krawedz:" << endl;
            bool bFirst = true;
            do
            {
                if (!bFirst) {
                    cout << "Krawedz zapisuje sie w postaci dwoch liczb naturalnych <= " + to_string(maxU) + " w formacie:\nout in\nSproboj ponownie:";
                    if (file) return false;
                }
                bFirst = false;
                n = 0;
                if ((daneWejscioweRaw = wprowadzDane("Wprowadzony ciag jest "
                    "niepoprawny. Wszystkie wartosci musza byc liczbami naturalnymi <= " + to_string(maxU) +
                    " i musi ich byc dokladnie 2", n)) == "" && file) return false;
            } while (n != 2);
            string word;
            stringstream readStream = stringstream(daneWejscioweRaw);
            readStream >> word;
            a = stoul(word);
            readStream >> word;
            b = stoul(word);
            n = 0;
            if (a >= rozmiarDanych){
                cout << "Niepoprawny wierzcholek \"" + to_string(a) + "\"" << endl;
                if (file) return false;
            }
            else if (b >= rozmiarDanych){
                cout << "Niepoprawny wierzcholek \"" + to_string(b) + "\"" << endl;
                if (file) return false;
            }
            else if (a == b){
                cout << "Wykryto petle wlasna \"" + to_string(a) + " " + to_string(b) + "\"" << endl;
                if (file) return false;
            }
            else if (count(listaNastepnikow[a].begin(), listaNastepnikow[a].end(), b)){
                cout << "Tworzenie multigrafu jest niedozwolone. Powtarzajaca sie krawedz \"" + to_string(a) + " " + to_string(b) + "\"" << endl;
                if (file) return false;
            }
            else if (!mode && macierzSasiedztwa[a][b])
            {
                cout << "Tworzenie multigrafu jest niedozwolone. Powtarzajaca sie krawedz \"" + to_string(a) + " " + to_string(b) + "\"" << endl;
                if (file) return false;
            }
            else bProper = true;
        }
        macierzSasiedztwa[b][a] = macierzSasiedztwa[a][b] = 1;
        listaNastepnikow[a].push_back(b);
    }
    if (!file) cout << endl;
    return true;
}

void wygenerujDane(int nasycenie)
{
    macierzSasiedztwa = usunmacierzSasiedztwa();
    listaNastepnikow = usunListeNastepnikow();
    mt19937 mt_rand(time(0));
    macierzSasiedztwa = new short int* [rozmiarDanych];
    listaNastepnikow = new vector<short int>[rozmiarDanych];
    for (unsigned i = 0; i != rozmiarDanych; ++i)
    {
        macierzSasiedztwa[i] = new short int[rozmiarDanych];
        for (unsigned j = 0; j != rozmiarDanych; ++j)
            macierzSasiedztwa[i][j] = 0;
    }

    liczbaKrawedzi = ((((rozmiarDanych * (rozmiarDanych - 1)) >> !mode)) * nasycenie)/10;
    if (mode)
    {
        bool** mozliwe = new bool* [rozmiarDanych];
        short int* indeg = new short int[rozmiarDanych], *outdeg = new short int[rozmiarDanych];
        int cantCount = 0;

        memset(indeg, 0, sizeof(short int) * rozmiarDanych);
        memset(outdeg, 0, sizeof(short int) * rozmiarDanych);
        for (int i = 0; i != rozmiarDanych; ++i)
        {
            mozliwe[i] = new bool[rozmiarDanych];
            memset(mozliwe[i], 0, sizeof(bool) * rozmiarDanych);
        }
        if (!mode)
        {
            for (unsigned i = 0; i != rozmiarDanych; ++i)
                for (unsigned j = i + 1; j != rozmiarDanych; ++j)
                    mozliwe[i][j] = 1;
        }
        else
            for (unsigned i = 0; i != rozmiarDanych; ++i)
                for (unsigned j = 0; j != rozmiarDanych; ++j)
                    if (i != j) mozliwe[i][j] = 1;

        for (int i = 0; i < liczbaKrawedzi; i += 2)
        {
            bool pasorzystnosc = false;
            unsigned first = 0;
            int cantNow = 0;
            for (int k = 0; k != rozmiarDanych; ++k)
            {
                if (outdeg[k] < indeg[k])
                    if (cantNow == cantCount)
                    {
                        first = k;
                        pasorzystnosc = true;
                        break;
                    }
                    else ++cantNow;
            }
            if (!pasorzystnosc)
            {
                uniform_int_distribution<unsigned> dane_rand(0u, rozmiarDanych - 1u);
                first = dane_rand(mt_rand);
            }

            do
            {
                bool done = false;
                for (int j = 0; j != rozmiarDanych; ++j)
                {
                    if (indeg[j] < outdeg[j] && mozliwe[first][j])
                    {
                        mozliwe[first][j] = 0;
                        listaNastepnikow[first].push_back(j);
                        macierzSasiedztwa[first][j] = macierzSasiedztwa[j][first] = 1;
                        done = true;
                        ++outdeg[first];
                        ++indeg[j];
                        cantCount = 0;
                        break;
                    }
                }
                if (!done)
                {
                    int j = 0;
                    for (; j != rozmiarDanych; ++j)
                    {
                        if (!mozliwe[first][j]) continue;
                        mozliwe[first][j] = 0;
                        listaNastepnikow[first].push_back(j);
                        macierzSasiedztwa[first][j] = macierzSasiedztwa[j][first] = 1;
                        ++outdeg[first];
                        ++indeg[j];
                        cantCount = 0;
                        break;
                    }
                    if (j == rozmiarDanych)
                    {
                        ++cantCount;
                        break;
                    }
                }
            } while (outdeg[first] != indeg[first]);
        }
        for (int k = 0; k != rozmiarDanych; ++k)
            delete[] mozliwe[k];
        delete[] mozliwe;
        delete[] outdeg;
        delete[] indeg;
    }
    else
    {
        bool **mozliwe = new bool*[rozmiarDanych];
        short int *stopnie = new short int[rozmiarDanych];
        int cantCount = 0;

        memset(stopnie, 0, sizeof(short int) * rozmiarDanych);
        for (int i = 0; i != rozmiarDanych; ++i)
        {
            mozliwe[i] = new bool[rozmiarDanych];
            memset(mozliwe[i], 0, sizeof(bool) * rozmiarDanych);
        }
        if (!mode)
        {
            for (unsigned i = 0; i != rozmiarDanych; ++i)
                for (unsigned j = i + 1; j != rozmiarDanych; ++j)
                    mozliwe[i][j] = 1;
        }
        else
            for (unsigned i = 0; i != rozmiarDanych; ++i)
                for (unsigned j = 0; j != rozmiarDanych; ++j)
                    if (i != j) mozliwe[i][j] = 1;

        for (int i = 0; i < liczbaKrawedzi; i+=2)
        {
            bool pasorzystnosc = false;
            unsigned first = 0;
            int cantNow = 0;
            for (int k = 0; k != rozmiarDanych; ++k)
            {
                if (stopnie[k] % 2)
                    if (cantNow == cantCount)
                    {
                        first = k;
                        pasorzystnosc = true;
                        break;
                    }
                    else ++cantNow;
            }
            if (!pasorzystnosc)
            {
                uniform_int_distribution<unsigned> dane_rand(0u, rozmiarDanych - 1u);
                first = dane_rand(mt_rand);
            }

            do
            {
                bool done = false;
                for (int j = 0; j != rozmiarDanych; ++j)
                {
                    if (stopnie[j] % 2 && mozliwe[first][j])
                    {
                        mozliwe[first][j] = 0;
                        listaNastepnikow[first].push_back(j);
                        macierzSasiedztwa[first][j] = macierzSasiedztwa[j][first] = 1;
                        done = true;
                        ++stopnie[first];
                        ++stopnie[j];
                        cantCount = 0;
                        break;
                    }
                }
                if (!done)
                {
                    int j = 0;
                    for (; j != rozmiarDanych; ++j)
                    {
                        if (!mozliwe[first][j]) continue;
                        mozliwe[first][j] = 0;
                        listaNastepnikow[first].push_back(j);
                        macierzSasiedztwa[first][j] = macierzSasiedztwa[j][first] = 1;
                        ++stopnie[first];
                        ++stopnie[j];
                        cantCount = 0;
                        break;
                    }
                    if (j == rozmiarDanych) 
                    {
                        ++cantCount;
                        break;
                    }
                }
            } while (stopnie[first] % 2);
        }
        for (int k = 0; k != rozmiarDanych; ++k)
            delete[] mozliwe[k];
        delete[] mozliwe;
        delete[] stopnie;
    }
}

bool wczytajDane()
{
    string wybor = "";
    if (!file) cout << "Wybierz rodzaj wprowadzanego grafu:\nn - nieskierowany\ns - skierowany" << endl;
    while (wybor != "n" && wybor != "s")
    {
        if (wybor.size())
        {
            cout << "Wybierz rodzaj wprowadzanego grafu:\nn - nieskierowany\ns - skierowany" << endl;
            if (file) return false;
        }
        getline(cin, wybor);
        //transform(wybor.begin(), wybor.end(), wybor.begin(), tolower);
    }
    if (wybor == "s")
        mode = 1;

    if (file)
    {
        if (!wprowadzDane())
        {
            cout << "Nie udalo sie wczytac danych" << flush;    //plik jest pusty albo źle sformatowany
            return false;
        }
    }
    else
    {
        cout << "Wybierz sposob wprowadzania danych:\nt - wprowadz dane recznie\ng - wygeneruj dane" << endl;
        wybor = "";
        while (wybor != "t" && wybor != "g")
        {
            if (wybor.size()) cout << "Wybierz sposob wprowadzania danych:\nt - wprowadz dane recznie\ng - wygeneruj dane" << endl;
            getline(cin, wybor);
            //transform(wybor.begin(), wybor.end(), wybor.begin(), tolower);
        }
        if (wybor == "t")
            wprowadzDane();
    }
    return true;
}