#include <math.h>
#include <chrono>
#include <ctime>
#include <cstring>
#include <iostream>
#include <fstream>
#include <sstream>
#include <random>
#include <algorithm>
#define WIELKOSC_DANYCH_MAX 4000
using namespace std;
int merge_porownania = 0;
void mergeSort(unsigned long long* tablica, int beg, int end)
{
    if (!(end - beg))
        return;

    int mid = (beg + end) / 2;
    mergeSort(tablica, beg, mid);
    mergeSort(tablica, mid + 1, end);

    int d = beg, i = 0, j = 0, max_size = 1 + (end - beg) / 2, max_sizePlus = (end - beg + 1) / 2;
    unsigned long long* tablicaPomocnicza1 = new unsigned long long[max_size];
    unsigned long long* tablicaPomocnicza2 = new unsigned long long[max_sizePlus];
    copy(tablica + beg, tablica + mid + 1, tablicaPomocnicza1);
    copy(tablica + mid + 1, tablica + end + 1, tablicaPomocnicza2);

    //scalanie
    while (true)
    {
        if (j == max_size)
        {
            while (i != max_sizePlus)
                tablica[d++] = tablicaPomocnicza2[i++];
            break;
        }

        if (i == max_sizePlus)
        {
            while (j != max_size)
                tablica[d++] = tablicaPomocnicza1[j++];
            break;
        }

        if (tablicaPomocnicza1[j] > tablicaPomocnicza2[i])
            tablica[d++] = tablicaPomocnicza1[j++];
        else
            tablica[d++] = tablicaPomocnicza2[i++];
        ++merge_porownania;
    }
    delete tablicaPomocnicza1;
    delete tablicaPomocnicza2;
}

int heap_operacje = 0;
int heap_porownania = 0;
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
            heap_operacje += 2;
            ++heap_porownania;
            maxKopiec(tablica, dziecko2, end);
        }
    }
    else if (tablica[rodzic] > tablica[dziecko1])
    {
        unsigned long long copy = tablica[rodzic];
        tablica[rodzic] = tablica[dziecko1];
        tablica[dziecko1] = copy;
        heap_operacje += 2;
        maxKopiec(tablica, dziecko1, end);
    }
    ++heap_porownania;
}

void heapSort(unsigned long long* tablica, int end)
{
    //utwórz kopiec
    for (int i = end; i != 0; --i)
        maxKopiec(tablica, i, end);

    while (end != 1)
    {
        unsigned long long copy = tablica[end];
        tablica[end] = tablica[1];
        tablica[1] = copy;
        heap_operacje += 2;
        --end;
        maxKopiec(tablica, 1, end);
    }
}

bool test = false, file = false;
int quick_operacje = 0;
int quick_porownania = 0;
string wybor = "";
void quickSort(unsigned long long* tablica, int beg, int end)
{
    if (beg >= end) return;
    int i = beg;
    {
        int j = end;
        unsigned long long pivot = tablica[end];
        while (true)
        {
            while (tablica[i] > pivot)
            {
                ++quick_porownania;
                if (++i > j) break;
            }
            while (tablica[j] < pivot)
            {
                ++quick_porownania;
                if (--j < beg) break;
            }
            if (i > j) break;
            unsigned long long copy = tablica[i];
            tablica[i++] = tablica[j];
            tablica[j--] = copy;
            quick_operacje += 2;
        }
        if (!test && !file && wybor[0] != 't') cout << "Pivot: " << pivot << endl;
        if (j > beg) quickSort(tablica, beg, j);
    }
    if (i < end) quickSort(tablica, i, end);
}

int main(int argc, char* args[])
{
    mt19937_64 mt_rand(time(0));    //64 bitowy generator liczb pseudolosowych
    uniform_int_distribution<int> n_rand(1000, WIELKOSC_DANYCH_MAX);
    uniform_int_distribution<unsigned long long> dane_rand(0, -1ull);
    cout << fixed;     //format wyświetlania ułamków dziesiętnych
    string max_wartosc = to_string(-1ull);  //użyte do sprawdzenia, czy użytkownik nie wpisał liczby przekraczającej zakres zmiennej unsigned long long
    for (int x = 0; x != argc; ++x)
    {
        if (!strcmp(args[x], "-test")) test = true;
        else if (!strcmp(args[x], "-file")) file = true;
    }
    //--------------LOGIKA PROGRAMU----------------
    int n = 0;
    unsigned long long* daneWejsciowe;
    string daneWejscioweRaw = "";
    if (file)
    {
        bool bDigit = true;
        getline(cin, daneWejscioweRaw);
        stringstream readStream(daneWejscioweRaw);
        string word;
        if (!daneWejscioweRaw.size())
        {
            cout << "Nie udalo sie wczytac danych.";    //plik jest pusty albo źle sformatowany
            return 0;
        }
        while (readStream >> word)
        {
            if (word.size() > max_wartosc.size()) bDigit = false;
            else if (word.size() == max_wartosc.size() && word > max_wartosc) bDigit = false;
            else
                for (char c : word)
                    if (!isdigit(c))
                        bDigit = false;
            ++n;
        }
        if (!bDigit)
        {
            cout << "Znaleziono nieprawidlowe dane";    //nie są liczbami bądź zawierają inne znaki niż cyfry
            return 0;
        }
        daneWejsciowe = new unsigned long long[n];
        readStream.clear();
        readStream.seekg(0, ios::beg);
        for (int i = 0; i != n; ++i)
        {
            readStream >> word;
            daneWejsciowe[i] = stoull(word);
        }
    }
    else
    {
        cout << "Wpisz 't', jesli chcesz wygenerowac losowo dane lub dowolny inny znak jezeli chcesz je wprowadzic. Pamietaj, ze wprowadzajac dane, ciag nie moze przekroczyc 10 znakow." << endl;
        while (!wybor.size()) getline(cin, wybor);
        if (wybor[0] != 't')
        {
            cout << "Podaj ciag liczb naturalnych. Oddzielaj liczby spacja: " << endl;
            bool bDigit = true;
            while (daneWejscioweRaw.size() == 0 || !bDigit)
            {
                n = 0;
                if (!bDigit) cout << "Wprowadzony ciag \"" + daneWejscioweRaw + "\" jest "
                    "niepoprawny. Wszystkie wartosci musza byc liczbami naturalnymi <= " +
                    max_wartosc + " i dlugosc ciagu nie moze przekraczac 10 znakow. Sproboj ponownie: " << endl;
                bDigit = true;
                getline(cin, daneWejscioweRaw);
                stringstream readStream(daneWejscioweRaw);
                string word;
                if (daneWejscioweRaw.size() == 0) continue;
                while (readStream >> word)
                {
                    if (word.size() > max_wartosc.size()) bDigit = false;
                    else if (word.size() == max_wartosc.size() && word > max_wartosc) bDigit = false;
                    else
                        for (char c : word)
                            if (!isdigit(c))
                                bDigit = false;
                    if (++n > 10) bDigit = false;
                }
                if (n == 0) bDigit = false;
                if (!bDigit) continue;
                daneWejsciowe = new unsigned long long[n];
                readStream.clear();
                readStream.seekg(0, ios::beg);
                for (int i = 0; i != n; ++i)
                {
                    readStream >> word;
                    daneWejsciowe[i] = stoull(word);
                }
            }
            cout << endl;
        }
        else
        {
            n = n_rand(mt_rand);
            daneWejsciowe = new unsigned long long[n];
            for (int i = 0; i != n; ++i)
                daneWejsciowe[i] = dane_rand(mt_rand);
        }
    }
    //--------------LOGIKA PROGRAMU----------------
    //--------------MERGE-SORT-----------------
    unsigned long long* copyDaneWejsciowe = new unsigned long long[n];
    copy(daneWejsciowe, daneWejsciowe + n, copyDaneWejsciowe);
    chrono::time_point<chrono::high_resolution_clock> start = chrono::high_resolution_clock::now();
    mergeSort(copyDaneWejsciowe, 0, n - 1);
    auto duration = chrono::duration_cast<chrono::microseconds>(chrono::high_resolution_clock::now() - start);
    if (!test) cout << "Wykonanie Merge-Sort trwalo: ";
    cout << duration.count();
    if (!test) cout << " mikrosekund";
    cout << endl;
    if (!test) cout << "Liczba wykonanych przyrownan elementow tablicy: ";
    cout << merge_porownania << endl;
    if (!test && !file && wybor[0] != 't')
    {
        cout << "Dane wejsciowe: " << daneWejscioweRaw << "\nDane wyjsciowe: ";
        for (int i = 0; i != n; ++i) cout << copyDaneWejsciowe[i] << " ";
        cout << endl;
    }
    if (!test) cout << endl;
    //--------------HEAP-SORT-----------------
    copy(daneWejsciowe, daneWejsciowe + n, copyDaneWejsciowe);
    start = chrono::high_resolution_clock::now();
    heapSort(copyDaneWejsciowe - 1, n);
    duration = chrono::duration_cast<chrono::microseconds>(chrono::high_resolution_clock::now() - start);
    if (!test) cout << "Wykonanie Heap-Sort trwalo: ";
    cout << duration.count();
    if (!test) cout << " mikrosekund";
    cout << endl;
    if (!test) cout << "Liczba wykonanych przyrownan elementow tablicy: ";
    cout << heap_porownania << endl;
    if (!test) cout << "Liczba wykonanych podstawien elementow tablicy: ";
    cout << heap_operacje << endl;
    if (!test && !file && wybor[0] != 't')
    {
        cout << "Dane wejsciowe: " << daneWejscioweRaw << "\nDane wyjsciowe: ";
        for (int i = 0; i != n; ++i) cout << copyDaneWejsciowe[i] << " ";
        cout << endl;
    }
    if (!test) cout << endl;
    //--------------INSERTION-SORT-----------------
    int insertion_operacje = 0, insertion_porownania = 0;
    copy(daneWejsciowe, daneWejsciowe + n, copyDaneWejsciowe);
    start = chrono::high_resolution_clock::now();
    for (int i = 1; i != n; ++i)
    {
        unsigned long long copy = copyDaneWejsciowe[i];
        int j = i - 1;
        for (; j != -1 && copyDaneWejsciowe[j] < copy; --j)
        {
            copyDaneWejsciowe[j + 1] = copyDaneWejsciowe[j];
            ++insertion_operacje += 2;
        }
        copyDaneWejsciowe[j + 1] = copy;
        ++insertion_porownania; //ostatnie porównanie trzeba uwzględnić
        ++insertion_operacje;
    }
    duration = chrono::duration_cast<chrono::microseconds>(chrono::high_resolution_clock::now() - start);
    if (!test) cout << "Wykonanie Insertion-Sort trwalo: ";
    cout << duration.count();
    if (!test) cout << " mikrosekund";
    cout << endl;
    if (!test) cout << "Liczba wykonanych przyrownan elementow tablicy: ";
    cout << insertion_porownania << endl;
    if (!test) cout << "Liczba wykonanych podstawien elementow tablicy: ";
    cout << insertion_operacje << endl;
    if (!test && !file && wybor[0] != 't')
    {
        cout << "Dane wejsciowe: " << daneWejscioweRaw << "\nDane wyjsciowe: ";
        for (int i = 0; i != n; ++i) cout << copyDaneWejsciowe[i] << " ";
        cout << endl;
    }
    if (!test) cout << endl;
    //--------------SHELL-SORT-----------------
    int shell_operacje = 0, shell_porownania = 0;
    copy(daneWejsciowe, daneWejsciowe + n, copyDaneWejsciowe);
    start = chrono::high_resolution_clock::now();
    int Knuth = 1;
    while (Knuth < n)
        Knuth = Knuth * 3 + 1;
    Knuth /= 9;
    if (!Knuth) Knuth = 1;
    int it = 1;
    while (Knuth)
    {
        for (int i = n - Knuth - 1; i >= 0; --i)
        {
            unsigned long long copy = copyDaneWejsciowe[i];
            int j = i + Knuth;
            for (; j < n && copyDaneWejsciowe[j] > copy; j += Knuth)
            {
                copyDaneWejsciowe[j - Knuth] = copyDaneWejsciowe[j];
                ++shell_operacje;
                ++shell_porownania;
            }
            copyDaneWejsciowe[j - Knuth] = copy;
            ++shell_operacje; //ostatnie porównanie trzeba uwzględnić
            ++shell_porownania;
        }
        if (!test && !file && wybor[0] != 't')
        {
            cout << "Iteracja nr " << it << " przyrost: " << Knuth << endl;
            ++it;
        }
        Knuth /= 3;
    }
    duration = chrono::duration_cast<chrono::microseconds>(chrono::high_resolution_clock::now() - start);
    if (!test) cout << "Wykonanie Shell-Sort trwalo: ";
    cout << duration.count();
    if (!test) cout << " mikrosekund";
    cout << endl;
    if (!test) cout << "Liczba wykonanych przyrownan elementow tablicy: ";
    cout << shell_porownania << endl;
    if (!test) cout << "Liczba wykonanych podstawien elementow tablicy: ";
    cout << shell_operacje << endl;
    if (!test && !file && wybor[0] != 't')
    {
        cout << "Dane wejsciowe: " << daneWejscioweRaw << "\nDane wyjsciowe: ";
        for (int i = 0; i != n; ++i) cout << copyDaneWejsciowe[i] << " ";
        cout << endl;
    }
    if (!test) cout << endl;
    //--------------QUICK-SORT-----------------
    copy(daneWejsciowe, daneWejsciowe + n, copyDaneWejsciowe);
    start = chrono::high_resolution_clock::now();
    quickSort(copyDaneWejsciowe, 0, n - 1);
    duration = chrono::duration_cast<chrono::microseconds>(chrono::high_resolution_clock::now() - start);
    if (!test) cout << "Wykonanie Quick-Sort trwalo: ";
    cout << duration.count();
    if (!test) cout << " mikrosekund";
    cout << endl;
    if (!test) cout << "Liczba wykonanych przyrownan elementow tablicy: ";
    cout << quick_porownania << endl;
    if (!test) cout << "Liczba wykonanych podstawien elementow tablicy: ";
    cout << quick_operacje << endl;
    if (!test && !file && wybor[0] != 't')
    {
        cout << "Dane wejsciowe: " << daneWejscioweRaw << "\nDane wyjsciowe: ";
        for (int i = 0; i != n; ++i) cout << copyDaneWejsciowe[i] << " ";
        cout << endl;
    }
    delete copyDaneWejsciowe;
    delete daneWejsciowe;
    //Press any key to exit the program
    if (!test) system("pause");
    return 0;
}

