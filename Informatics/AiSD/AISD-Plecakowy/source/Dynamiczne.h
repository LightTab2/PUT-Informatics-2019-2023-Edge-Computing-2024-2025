#pragma once
#include "ZmienneGlobalne.h"

void APD(Przedmiot* dane = przedmioty, unsigned long long przed = liczba_przedmiotow, unsigned long long size = pojemnosc_plecaka)
{
	++przed;
	++size;
	unsigned long long **macierz = new unsigned long long*[przed];
	for (unsigned long long i = 0ull; i != przed; ++i)
	{
		macierz[i] = new unsigned long long [size];
		memset(macierz[i], 0ull, sizeof(unsigned long long) * size);
	}

	for (unsigned long long i = 1ull; i != przed; ++i)
		for (unsigned long long j = 1ull; j != size; ++j)
			if (dane[i-1ull].waga > j)
				macierz[i][j] = macierz[i-1ull][j];
			else
				macierz[i][j] = max(macierz[i-1ull][j], macierz[i-1ull][j - dane[i-1ull].waga] + dane[i-1ull].cena);

	if (!test)
	{
		unsigned long long waga_sum = 0;

		if (!macierz[przed - 1ull][size - 1ull])
			cout << "APD: Brak miejsca na przedmioty" << endl;
		else
		{
			cout << "APD: Obiekty w plecaku (id): ";

			for (unsigned long long i = przed - 1ull, j = size - 1ull; i; --i)
				if ((macierz[i][j] - macierz[i - 1ull][j]))
				{
					cout << i << ' ';
					j -= dane[i - 1ull].waga;
					waga_sum += dane[i - 1ull].waga;
				}
			cout << "\nWartosc przedmiotow lacznie: " << macierz[przed-1ull][size - 1ull] << endl;
			cout << "Waga przedmiotow lacznie: " << waga_sum << endl;
		}
	}

	for (unsigned long long i = 0ull; i != przed; ++i)
		delete[] macierz[i];
	delete[] macierz;
}