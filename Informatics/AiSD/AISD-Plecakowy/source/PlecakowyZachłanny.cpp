#pragma once
#include <iostream>
#include <sstream>
#include <stdio.h>
#include <random>
#include <chrono>
#include <algorithm>
#include <string>
using namespace std;
struct Przedmiot
{
    unsigned long long waga, cena;
};

//Limity danych
unsigned long long maxULL = static_cast<unsigned long long>(-1);
unsigned maxU = static_cast<unsigned>(-1);

bool test = false, file = false;	//przełączniki

//Ilosc Przedmiotów oraz pojemnosc plecaka
unsigned long long liczba_przedmiotow = 0;
unsigned long long pojemnosc_plecaka = 0;

//Tablica przedmiotow
Przedmiot* przedmioty;

unsigned long long* selection_sort(struct Przedmiot* przetwarzane_przedmioty, unsigned long long ilosc)
{
    unsigned long long* sorted_tab = new unsigned long long[ilosc];
    double* wspolczynnik = new double[ilosc];
    unsigned long long temp;
    unsigned long long temp_wartosc;
    for (unsigned long long i = 0; i < ilosc; i++)
    {
        sorted_tab[i] = i;
        if (przetwarzane_przedmioty[i].waga)
            wspolczynnik[i] = przetwarzane_przedmioty[i].cena / (double)przetwarzane_przedmioty[i].waga;
        else
            wspolczynnik[i] = numeric_limits<double>::max();
    }
    for (unsigned long long i = 0; i < ilosc; i++)
    {
        temp = i;
        for (unsigned long long j = i + 1; j < ilosc; j++)
        {
            if (wspolczynnik[sorted_tab[j]] > wspolczynnik[sorted_tab[temp]])
            {
                temp = j;
            }
        }
        temp_wartosc = sorted_tab[i];
        sorted_tab[i] = sorted_tab[temp];
        sorted_tab[temp] = temp_wartosc;
    }
    delete[] wspolczynnik;
    return sorted_tab;
}

void AZ(struct Przedmiot* przetwarzane_przedmioty = przedmioty, unsigned long long ilosc = liczba_przedmiotow, unsigned long long miejsce = pojemnosc_plecaka)
{
    if (!miejsce)
    {
        if (!test)
            cout << "AZ: Brak miejsca na przedmioty" << endl;
        return;
    }
    unsigned long long waga = 0;
    unsigned long long wartosc = 0;
    unsigned long long* plecak = new unsigned long long[ilosc];
    unsigned long long plecak_head = 0;
    unsigned long long* sorted = selection_sort(przetwarzane_przedmioty, ilosc);
    for (unsigned long long i = 0; i < ilosc; i++)
    {
        if (waga + przetwarzane_przedmioty[sorted[i]].waga <= miejsce)
        {
            plecak[plecak_head] = sorted[i];
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
                cout << plecak[i] + 1 << " ";
            cout << "\nWartosc przedmiotow lacznie: " << wartosc << "\n";
            cout << "Waga przedmiotow lacznie: " << waga << "\n";
        }
        else
            cout << "AZ: Brak miejsca na przedmioty." << endl;
    }
    delete[] plecak;
    delete[] sorted;
    return;
}
//Sprawdza czy dane są liczbami oraz czy zawierają się w przedziale <min; limit>
bool poprawnoscDanych(stringstream& readStream, unsigned long long& n, unsigned long long limit = static_cast<unsigned long long>(-1), unsigned long long min = 0ull)
{
    string maxVal = to_string(limit);
    string minVal = to_string(min);
    n = 0ull;
    string word;
    bool bDigit = true;
    while (bDigit && readStream >> word)
    {
        if (word.size() > maxVal.size()) bDigit = false;
        else if (word.size() < minVal.size()) bDigit = false;
        else if (word.size() == minVal.size() && word < minVal) bDigit = false;
        else if (word.size() == maxVal.size() && word > maxVal) bDigit = false;
        else
            for (char c : word)
                if (!isdigit(c))
                    bDigit = false;
        if (n++ > limit) bDigit = false;
    }
    if (n == 0ull) bDigit = false;
    readStream.clear();
    readStream.seekg(0, ios::beg);
    return bDigit;
}

//Error zostanie wyswietlony, gdy uzytkownik wprowadzi niepoprawne dane. Parametr &n zostanie ustawiony na liczbę równej liczności wczytanych elementów.
//min i limit zostaną przekazane do poprawnoscDanych(...)
string wprowadzDane(string error, unsigned long long& n, unsigned long long limit = static_cast<unsigned long long>(-1), unsigned long long min = 0ull)
{
    string wybor = "", word;
    bool bDigit = true;
    while (wybor.size() == 0 || !bDigit)
    {
        if (!bDigit)
        {
            cout << error << endl;
            if (file) return "";
        }
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
    if (!file) cout << "Podaj dwie liczby naturalne liczbe przedmiotow i pojemnosc plecaka:" << endl;

    unsigned long long n = 0;
    while (n != 2)
    {
        if (n != 0) 
        {
            cout << "Nalezy podac dokladnie dwie liczby" << endl;
            if (file) return false;
        }
        if ((daneWejscioweRaw = wprowadzDane("Wprowadzony ciag jest "
                                                "niepoprawny. Wszystkie wartosci musza byc liczbami naturalnymi <= " + to_string(maxULL) + ". Sproboj ponownie:", n)) == "" && file) return false;
    }
    n = 0;
    string word;
    stringstream readStream = stringstream(daneWejscioweRaw);
    readStream >> word;
    liczba_przedmiotow = stoull(word);
    readStream >> word;
    pojemnosc_plecaka = stoull(word);

    przedmioty = new Przedmiot[liczba_przedmiotow];
    for (unsigned long long i = 0; i != liczba_przedmiotow; ++i)   //tutaj są wczytywane przedmioty
    {
        if (!file) cout << "\nPodaj " + to_string(i + 1) + ". przedmiot:" << endl;
        n = 0;
        do
        {
            if (n) 
            {
                cout << "Przedmiot zapisuje sie w postaci dwoch liczb naturalnych <= " + to_string(maxULL) + " w formacie:\nwaga wartosc\nSproboj ponownie:" << endl;
                if (file) return false;
            }
            if ((daneWejscioweRaw = wprowadzDane("Wprowadzony ciag jest niepoprawny. Wszystkie wartosci musza byc liczbami naturalnymi <= "
                                                 + to_string(maxULL) + " i musi ich byc dokladnie 2", n)) == "" && file) return false;
        } while (n != 2);
        string word;
        stringstream readStream = stringstream(daneWejscioweRaw);
        readStream >> word;
        przedmioty[i].waga = stoull(word);
        readStream >> word;
        przedmioty[i].cena = stoull(word);
    }
    if (!file) cout << endl;
    return true;
}

void wygenerujDane()
{
    string daneWejscioweRaw = "";
    mt19937 mt_rand(time(0));
    cout << "\nPodaj liczbe przedmiotow (0 >= n <= " + to_string(1000ull) + "):" << endl;
    unsigned long long n = 0;
    while (true)
    {
        daneWejscioweRaw = wprowadzDane("Sproboj ponownie: ", n, 1000ull);
        if (n != 1) cout << "Musisz podac dokladnie jedna liczbe. Sproboj ponownie: ";
        else break;
    }

    string word;
    stringstream readStream = stringstream(daneWejscioweRaw);
    readStream >> word;
    liczba_przedmiotow = stoull(word);
    przedmioty = new Przedmiot[liczba_przedmiotow];
    cout << "Podaj pojemnosc plecaka:" << endl;

    daneWejscioweRaw = wprowadzDane("Sproboj ponownie: ", n);
    readStream = stringstream(daneWejscioweRaw);
    readStream >> word;
    pojemnosc_plecaka = stoull(word);

    for (unsigned long long i = 0; i != liczba_przedmiotow; ++i)
    {
        uniform_int_distribution<unsigned long long> dane_rand(pojemnosc_plecaka/liczba_przedmiotow >> 1, (pojemnosc_plecaka << 2)/liczba_przedmiotow);
        uniform_int_distribution<unsigned long long> dane_rand2(1, 20);
        przedmioty[i].waga = max(1ull, dane_rand(mt_rand));
        przedmioty[i].cena = przedmioty[i].waga * dane_rand2(mt_rand);  
        cout << i + 1 << ": " << przedmioty[i].waga << " " << przedmioty[i].cena << endl;
    }
}

bool wczytajDane()
{
    string wybor = "";

    if (file)
    {
        if (!wprowadzDane())
        {
            cout << "Nie udalo sie wczytac danych" << endl;    //plik jest pusty albo źle sformatowany
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
        else if (wybor == "g")
            wygenerujDane();
    }
    return true;
}


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
        cout << "Znaleziono nieprawidlowe dane" << endl;
        return 0;
    }

    auto start = chrono::high_resolution_clock::now();
    AZ();
    auto duration = chrono::duration_cast<chrono::microseconds>(chrono::high_resolution_clock::now() - start);
    if (test) cout << duration.count() << endl;
    else cout << endl;

    if (!test) cout << "\a" << endl;
    //--------------KONIEC PROGRAMU----------------
    //ZWOLNIENIE PAMIECI
    delete[] przedmioty;
    if (!test) system("pause");
    return 0;
}
