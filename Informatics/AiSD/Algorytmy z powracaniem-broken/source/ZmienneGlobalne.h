#pragma once
#include <sstream>
#include <iostream>
#include <stdio.h>
#include <random>
#include <chrono>
#include <algorithm>
#include <string>

using namespace std;

//Limity danych
unsigned long long maxULL = static_cast<unsigned long long>(-1);
unsigned maxU = static_cast<unsigned>(-1);

bool test = false, file = false;	//przełączniki

//Liczba wierzchołków i krawędzi
unsigned int rozmiarDanych = 0, liczbaKrawedzi = 0;

//Użyte przy wykrywaniu cykli w grafie
bool mode = 0;	

//Reprezentacje grafów
short int** macierzSasiedztwa = nullptr;
vector<short int> *listaNastepnikow = nullptr;
chrono::time_point<chrono::high_resolution_clock> start;
