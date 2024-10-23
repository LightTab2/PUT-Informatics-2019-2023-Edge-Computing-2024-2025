#include <iostream>
#include <fstream>
#include <ctime>
#include <random>
#include <algorithm>
#define N 15
#define BASE 200
#define GROWTH 200
#define N_PLECAK 15
#define BASE_PLECAK 10000
#define GROWTH_PLECAK 2000

using namespace std;
int main(int argc, char* args[])
{
    ios_base::sync_with_stdio(0);
    cin.tie(0);
    ofstream plik;
    unsigned wielkosc_danych[N];
    unsigned wielkosc_plecaka[N_PLECAK];
    for (unsigned i = 0; i != N; ++i)
        wielkosc_danych[i] = BASE + i * GROWTH;

    for (unsigned i = 0; i != N_PLECAK; ++i)
        wielkosc_plecaka[i] = BASE_PLECAK + i * GROWTH_PLECAK;

    mt19937 mt_rand(time(0));

    for (int pomiar = 0; pomiar != 5; ++pomiar)
    {
        for (int i = 0; i != 15; ++i)
        {
            for (int j = 0; j != 15; ++j)
            {
                plik.open("..\\plec_" + to_string(pomiar) + '_' + to_string(j) + '_' + to_string(i));
                unsigned liczba_przedmiotow = wielkosc_danych[i], pojemnosc_plecaka = wielkosc_plecaka[j];
                string daneWejscioweRaw = "";
                mt19937 mt_rand(time(0));

                plik << liczba_przedmiotow << ' ' << pojemnosc_plecaka << endl;
                for (unsigned long long i = 0; i != liczba_przedmiotow; ++i)
                {
                    uniform_int_distribution<unsigned> dane_rand(pojemnosc_plecaka / liczba_przedmiotow >> 1, (pojemnosc_plecaka << 2) / liczba_przedmiotow);
                    uniform_int_distribution<unsigned> dane_rand2(1, 20);
                    unsigned waga = dane_rand(mt_rand);
                    if (!waga) waga = 1;
                    plik << waga << ' ' << waga * dane_rand2(mt_rand) << endl;
                }
                plik.close();
            }
        }
    }
    //Press any key to exit the program
    return 0;
}
