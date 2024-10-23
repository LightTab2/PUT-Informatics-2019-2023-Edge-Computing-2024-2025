#include <iostream>
#include <fstream>
#include <ctime>
#include <random>
using namespace std;

int wielkosc_danych[] = { 2599, 4199, 5799, 7399, 8999, 10599, 12199, 13799, 15399, 16999, 18599, 20199, 21799, 23399, 24999};
int main(int argc, char* args[])
{
    ofstream plik;
    srand(time(0));
    for (int pomiar = 0; pomiar != 5; ++pomiar)
    {
        //Losowe
        mt19937_64 mt_rand(rand());    //64 bitowy generator liczb pseudolosowych
        for (int i = 0; i != 15; ++i)
        {
            unsigned long long min = 0ull, max = -1ull;
            plik.open("..\\ran_" + to_string(pomiar) + '_' + to_string(i));
            for (int j = 0; j != wielkosc_danych[i]; ++j)
            {
                uniform_int_distribution<unsigned long long> dane_rand(min, max);
                plik << dane_rand(mt_rand) << ' ';
            }
            plik.close();
        
            plik.open("..\\ros_" + to_string(pomiar) + '_' + to_string(i));
            for (int j = 0; j != wielkosc_danych[i]; ++j)
            {
                uniform_int_distribution<unsigned long long> dane_rand(min, max - wielkosc_danych[i] + j);
                min = dane_rand(mt_rand);  
                plik << min++ << ' ';
            }
            plik.close();
            min = 0ull;

            plik.open("..\\mal_" + to_string(pomiar) + '_' + to_string(i));
            for (int j = 0; j != wielkosc_danych[i]; ++j)
            {
                uniform_int_distribution<unsigned long long> dane_rand(min + wielkosc_danych[i] - j, max);
                max = dane_rand(mt_rand);  
                plik << max-- << ' ';
            }
            plik.close();
            max = -1ull;

            uniform_int_distribution<unsigned long long> first_rand(min, max);
            unsigned long long *array = new unsigned long long[wielkosc_danych[i]];
            max = array[wielkosc_danych[i]/2] = first_rand(mt_rand);
            --max;
            plik.open("..\\A_" + to_string(pomiar) + '_' + to_string(i));
            for (int j = 1; j != wielkosc_danych[i]/2 + 1; ++j)
            {
                uniform_int_distribution<unsigned long long> dane_rand(min + wielkosc_danych[i]/2 - j, max);
                array[wielkosc_danych[i]/2 - j] = dane_rand(mt_rand); 
                array[wielkosc_danych[i]/2 + j] = dane_rand(mt_rand); 
                max = (array[wielkosc_danych[i]/2 - j] > array[wielkosc_danych[i]/2 + j]) ? array[wielkosc_danych[i]/2 + j] : array[wielkosc_danych[i]/2 - j];  
                --max;
            }
            for (int k = 0; k != wielkosc_danych[i]; ++k)
                plik << array[k] << ' ';
            plik.close();
            max = -1ull;

            uniform_int_distribution<unsigned long long> first_rand2(min, max);
            min = array[wielkosc_danych[i]/2] = first_rand2(mt_rand);
            ++min;
            plik.open("..\\V_" + to_string(pomiar) + '_' + to_string(i));
            for (int j = 1; j != wielkosc_danych[i]/2 + 1; ++j)
            {
                uniform_int_distribution<unsigned long long> dane_rand(min + wielkosc_danych[i]/2 - j, max);
                array[wielkosc_danych[i]/2 - j] = dane_rand(mt_rand); 
                array[wielkosc_danych[i]/2 + j] = dane_rand(mt_rand); 
                min = (array[wielkosc_danych[i]/2 - j] > array[wielkosc_danych[i]/2 + j]) ? array[wielkosc_danych[i]/2 - j] : array[wielkosc_danych[i]/2 + j];
                ++min;
            }
            for (int k = 0; k != wielkosc_danych[i]; ++k)
                plik << array[k] << ' ';
            plik.close();
            delete array;
        }
    }
    //Press any key to exit the program
    return 0;
}
