#pragma once
#include "ZmienneGlobalne.h"
bool* bOdwiedzone;
unsigned* droga, dlugoscDrogi;

bool algorytmRobertsaFloresa_skierowany(unsigned wierzcholek)
{
	bOdwiedzone[wierzcholek] = true;
	if (++dlugoscDrogi == rozmiarDanych && count(listaNastepnikow[wierzcholek].cbegin(), listaNastepnikow[wierzcholek].cend(), 0))
		return true;

	for (auto it = listaNastepnikow[wierzcholek].cbegin(); it != listaNastepnikow[wierzcholek].cend(); ++it)
	{
		if (bOdwiedzone[*it]) continue;
		if (algorytmRobertsaFloresa_skierowany(*it))
		{
			droga[--dlugoscDrogi] = *it;
			return true;
		}
	}
	bOdwiedzone[wierzcholek] = false;
	--dlugoscDrogi;
	unsigned rozmiarKwadrat = rozmiarDanych * rozmiarDanych * 10;
	if (chrono::duration_cast<chrono::microseconds>(chrono::high_resolution_clock::now() - start).count() > rozmiarKwadrat) return true;
	return false;
}

void znajdzCyklHamiltona_skierowany()
{
	bOdwiedzone = new bool[rozmiarDanych];
	droga = new unsigned[rozmiarDanych];
	droga[0] = 0;
	dlugoscDrogi = 0;
	for (unsigned i = 0; i != rozmiarDanych; ++i)
		bOdwiedzone[i] = false;

	if (!algorytmRobertsaFloresa_skierowany(0))
	{
		delete[] bOdwiedzone;
		delete[] droga;
		return;
	}

	delete[] bOdwiedzone;
	if (!test)
	{
		for (unsigned i = 0; i != rozmiarDanych; ++i)
			cout << droga[i] << ' ';
		cout << '0';
	}
	delete[] droga;
}


bool algorytmRobertsaFloresa_nieskierowany(unsigned wierzcholek)
{
	//if (rozmiarDanych < 3) return false;
	bOdwiedzone[wierzcholek] = true;
	if (++dlugoscDrogi == rozmiarDanych && macierzSasiedztwa[wierzcholek][0])
		return true;

	for (auto i = 0; i != rozmiarDanych; ++i)
	{
		if (bOdwiedzone[i]) continue;
		if (macierzSasiedztwa[wierzcholek][i] && algorytmRobertsaFloresa_nieskierowany(i))
		{
			droga[--dlugoscDrogi] = i;
			return true;
		}
	}
	bOdwiedzone[wierzcholek] = false;
	--dlugoscDrogi;
	unsigned rozmiarKwadrat = rozmiarDanych * rozmiarDanych * 10;
	if (chrono::duration_cast<chrono::microseconds>(chrono::high_resolution_clock::now() - start).count() > rozmiarKwadrat) return true;
	return false;
}

void znajdzCyklHamiltona_nieskierowany()
{
	bOdwiedzone = new bool[rozmiarDanych];
	droga = new unsigned[rozmiarDanych];
	droga[0] = 0;
	dlugoscDrogi = 0;
	for (unsigned i = 0; i != rozmiarDanych; ++i)
		bOdwiedzone[i] = false;

	if (!algorytmRobertsaFloresa_nieskierowany(0))
	{
		delete[] bOdwiedzone;
		delete[] droga;
		return;
	}

	delete[] bOdwiedzone;
	if (!test)
	{
		for (unsigned i = 0; i != rozmiarDanych; ++i)
			cout << droga[i] << ' ';
		cout << '0';
	}
	delete[] droga;
}

void znajdzCyklHamiltona()
{
	if (mode) znajdzCyklHamiltona_skierowany();
	else znajdzCyklHamiltona_nieskierowany();
}