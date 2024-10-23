#pragma once
#include <sstream>
#include <iostream>
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