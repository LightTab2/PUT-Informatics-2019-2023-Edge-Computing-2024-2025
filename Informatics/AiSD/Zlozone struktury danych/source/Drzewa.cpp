#include "Drzewa.h"
#include "Sortowania.h"
#include "ModyfikowanieBST.h"
#include "ModyfikowanieAVL.h"
#include "WyswietlanieBST.h"
#include "WyswietlanieAVL.h"
#include "RotacjeBST.h"
using namespace std;

bool poprawnoscDanych(stringstream& readStream, unsigned int &n, unsigned long long limit, string maxVal);
void maxKopiec(unsigned long long* tablica, int rodzic, int end);
void heapSort(unsigned long long* tablica, int end);
bool wczytajDane();
string wprowadzDane();

int main(int argc, char* args[])
{
    cout << fixed;
    for (int x = 0; x != argc; ++x)
    {
        if (!strcmp(args[x], "-test")) test = true;
        else if (!strcmp(args[x], "-file")) file = true;
    }
    //--------------LOGIKA PROGRAMU----------------
    unsigned long long n = 0;
    if (!wczytajDane()) return 0;
    //--------------LOGIKA PROGRAMU----------------
    bool run = true;
    while (run)
    {
        string wybor = "";
        while (wybor.size() != 1 || (wybor != "a" && wybor != "b" && wybor!= "c"))
        {
            if (!test) cout << "Wybierz drzewo, na ktorym chcesz operowac\na - AVL\nb - BST\nc - AVL binarnie utworzone" << endl;
            getline(cin, wybor);
        }
        if (wybor == "a")
        {
            if (!test) cout << "Wybierz procedure do wykonania:\n1 - wyszukanie w drzewie elementu o najmniejszej i najwiekszej wartosci i wypisanie sciezki poszukiwania\n"
                "2 - usuniecie elementu drzewa o wartosci klucza podanego przez uzytkownika ( bez rownowazenia )\n3 - wypisanie wszystkich elementow drzewa w porzadku rosnacym (in-order)\n"
                "4 - wypisanie wszystkich elementow drzewa w porzadku pre-order\n5 - usuniecie calego drzewa element po elemencie metoda post-order\n"
                "6 - wypisanie w porzadku pre-order podrzewa, ktorego korzen (klucz) podaje uzytkownik\n7 - usuniecie elementu drzewa o wartosci klucza podanego przez uzytkownika ( z rownowazeniem )\n"
                "8 - wyswietlenie drzewa\n9 - zakonczenie programu" << endl;
            
            wybor = "";
            loop:do
            {
                if (wybor.size() != 0) cout << "Podano nieprawidlowe znaki. Sproboj ponownie: " << endl;
                getline(cin, wybor);
            } while (wybor.size() != 1);

            //system("cls");
            switch (wybor[0])
            {
            case '1':
                if (!avl)
                    cout << "Drzewo jest puste" << endl;
                else
                {
                    if (!test) cout << "Sciezka minimum: " << endl;
                    chrono::time_point<chrono::high_resolution_clock> start = chrono::high_resolution_clock::now();
                    unsigned long long minVar = minAVL(avl , avl_zrownowazone);
                    auto duration = chrono::duration_cast<chrono::microseconds>(chrono::high_resolution_clock::now() - start);
                    if (test)
                        cout << duration.count() << endl;
                    if (!test) cout << "Minimum AVL: " << minVar << "\nSciezka maksimum: " << endl;
                    unsigned long long maxVar = maxAVL("" , avl , avl_zrownowazone);
                    if (!test)
                    {
                        cout << avl->val; 
                        if (avl_zrownowazone) cout << '[' << avl->rownowaga << ']'; cout << "\nMaksimum AVL: " << maxVar << endl;
                    }
                }
                break;
            case '2':
                if (!avl)
                    cout << "Drzewo jest puste" << endl;
                else
                {
                    cout << "Podaj liczbe wezlow do usuniecia i nazwy: " << endl;
                    stringstream readStream = stringstream(wprowadzDane());
                    string word;
                    readStream >> word;
                    n = stoull(word);
                    for (unsigned long long i = 0; i != n && avl; ++i)
                    {
                        if (!(readStream >> word)) break;
                        usunAVL_bezrownowagi(stoull(word));
                    }
                    avl_zrownowazone = false;
                    rysujAVL("" , "" , avl , avl_zrownowazone );
                }
                break;
            case '3':
                if (!avl)
                    cout << "Drzewo jest puste" << endl;
                else
                {
                    chrono::time_point<chrono::high_resolution_clock> start = chrono::high_resolution_clock::now();
                    wypiszInOrderAVL();
                    if (!test) cout << endl;
                    auto duration = chrono::duration_cast<chrono::microseconds>(chrono::high_resolution_clock::now() - start);
                    if (test)
                        cout << duration.count() << endl;
                }
                break;
            case '4':
                if (!avl)
                    cout << "Drzewo jest puste" << endl;
                else
                {
                    wypiszPreOrderAVL();
                    cout << endl;
                }
                break;
            case '5':
                if (!avl)
                    cout << "Drzewo jest puste" << endl;
                else
                {
                    karczujAVL();
                    cout << "Usuniecie wykonano pomyslnie" << endl;
                    avl = nullptr;
                }
                break;
            case '6':
                if (!avl)
                    cout << "Drzewo jest puste" << endl;
                else
                {
                    AVL* drzewo = avl;
                    cout << "Podaj klucz: ";
                    unsigned long long dane = stoull(wprowadzDane());
                    while (drzewo->val != dane)
                    {
                        if (drzewo->val <= dane)
                            drzewo = drzewo->right;
                        else drzewo = drzewo->left;
                        if (!drzewo) break;
                    }
                    if (!drzewo)
                    {
                        cout << "Nie znaleziono wezla o podanej wartosci" << endl;
                        break;
                    }
                    wypiszPreOrderAVL(drzewo);
                    cout << endl;
                }
                break;
            case '7':
                if(!avl)
                    cout << "Drzewo jest puste" << endl;
                else
                {
                    if(!avl_zrownowazone)
                    {
                        cout << "Drzewo jest niezrownowazone" << endl;
                        break;
                    }
                    cout << "Podaj liczbe wezlow do usuniecia i nazwy: " << endl;
                    stringstream readStream = stringstream(wprowadzDane());
                    string word;
                    readStream >> word;
                    n = stoull(word);
                    for (unsigned long long i = 0; i != n && avl; ++i)
                    {
                        if (!(readStream >> word)) break;
                        usunAVL(stoull(word));
                    }
                    rysujAVL("" , "" , avl , avl_zrownowazone );
                }
                    break;
            case '8':
                if (!avl)
                    cout << "Drzewo jest puste" << endl;
                else rysujAVL("" , "" , avl , avl_zrownowazone);
                break;
            case '9':
                run = false;
                break;
            default:
                goto loop;
                break;
            }
        }
        else
        if(wybor == "b")
        {
            if (!test) cout << "Wybierz procedure do wykonania:\n1 - wyszukanie w drzewie elementu o najmniejszej i najwiekszej wartosci i wypisanie sciezki poszukiwania\n"
                "2 - usuniecie elementu drzewa o wartosci klucza podanego przez uzytkownika\n3 - wypisanie wszystkich elementow drzewa w porzadku rosnacym (in-order)\n"
                "4 - wypisanie wszystkich elementow drzewa w porzadku pre-order\n5 - usuniecie calego drzewa element po elemencie metoda post-order\n"
                "6 - wypisanie w porzadku pre-order podrzewa, ktorego korzen (klucz) podaje uzytkownik\n7 - algorytm DSW\n"
                "8 - wyswietlenie drzewa\n9 - zakonczenie programu" << endl;
            wybor = "";

            loop2:do
            {
                if (wybor.size() != 0) cout << "Podano nieprawidlowe znaki. Sproboj ponownie: " << endl;
                getline(cin, wybor);
            } while (wybor.size() != 1);

            //system("cls");
            switch (wybor[0])
            {
            case '1':
                if (!bst)
                    cout << "Drzewo jest puste" << endl;
                else
                {
                    if (!test) cout << "Sciezka minimum: " << endl;
                    chrono::time_point<chrono::high_resolution_clock> start = chrono::high_resolution_clock::now();
                    unsigned long long minVar = minBST();
                    auto duration = chrono::duration_cast<chrono::microseconds>(chrono::high_resolution_clock::now() - start);
                    if (test)
                        cout << duration.count() << endl;
                    if (!test) cout << "Minimum BST: " << minVar << "\nSciezka maksimum: " << endl;
                    unsigned long long maxVar = maxBST();
                    if (!test) cout << bst->val << "\nMaksimum BST: " << maxVar << endl;
                }
                break;
            case '2':
                if (!bst)
                    cout << "Drzewo jest puste" << endl;
                else
                {
                    cout << "Podaj liczbe wezlow do usuniecia i nazwy: " << endl;
                    stringstream readStream = stringstream(wprowadzDane());
                    string word;
                    readStream >> word;
                    n = stoull(word);
                    for (unsigned long long i = 0; i != n && bst; ++i)
                    {
                        if (!(readStream >> word)) break;
                        usunBST(stoull(word));
                    }
                    rysujBST();
                }
                break;
            case '3':
                if (!bst)
                    cout << "Drzewo jest puste" << endl;
                else
                {
                    chrono::time_point<chrono::high_resolution_clock> start = chrono::high_resolution_clock::now();
                    wypiszInOrderBST();
                    if (!test) cout << endl;
                    auto duration = chrono::duration_cast<chrono::microseconds>(chrono::high_resolution_clock::now() - start);
                    if (test)
                        cout << duration.count() << endl;
                }
                break;
            case '4':
                if (!bst)
                    cout << "Drzewo jest puste" << endl;
                else
                {
                    wypiszPreOrderBST();
                    cout << endl;
                }
                break;
            case '5':
                if (!bst)
                    cout << "Drzewo jest puste" << endl;
                else
                {
                    karczujBST();
                    cout << "Usuniecie wykonano pomyslnie" << endl;
                    bst = nullptr;
                }
                break;
            case '6':
                if (!bst)
                    cout << "Drzewo jest puste" << endl;
                else
                {
                    BST* drzewo = bst;
                    cout << "Podaj klucz: ";
                    unsigned long long dane = stoull(wprowadzDane());
                    while (drzewo->val != dane)
                    {
                        if (drzewo->val <= dane)
                            drzewo = drzewo->right;
                        else drzewo = drzewo->left;
                        if (!drzewo) break;
                    }
                    if (!drzewo)
                    {
                        cout << "Nie znaleziono wezla o podanej wartosci" << endl;
                        break;
                    }
                    wypiszPreOrderBST(drzewo);
                    cout << endl;
                }
                break;
            case '7':
                if (!bst)
                    cout << "Drzewo jest puste" << endl;
                else
                {
                    chrono::time_point<chrono::high_resolution_clock> start = chrono::high_resolution_clock::now();
                    DSW_BST();
                    auto duration = chrono::duration_cast<chrono::microseconds>(chrono::high_resolution_clock::now() - start);
                    if (test)
                        cout << duration.count() << endl;
                    if (!test) rysujBST();
                }
                break;
            case '8':
                if (!bst)
                    cout << "Drzewo jest puste" << endl;
                else rysujBST();
                break;
            case '9':
                run = false;
                break;
            default:
                goto loop2;
                break;
            }
        }
        else
        {
            cout << "Wybierz procedure do wykonania:\n1 - wyszukanie w drzewie elementu o najmniejszej i najwiekszej wartosci i wypisanie sciezki poszukiwania\n"
                    "2 - usuniecie elementu drzewa o wartosci klucza podanego przez uzytkownika ( bez rownowazenia )\n3 - wypisanie wszystkich elementow drzewa w porzadku rosnacym (in-order)\n"
                    "4 - wypisanie wszystkich elementow drzewa w porzadku pre-order\n5 - usuniecie calego drzewa element po elemencie metoda post-order\n"
                    "6 - wypisanie w porzadku pre-order podrzewa, ktorego korzen (klucz) podaje uzytkownik\n"
                    "8 - wyswietlenie drzewa\n9 - zakonczenie programu" << endl;

            wybor = "";
            loop3:do
            {
                if (wybor.size() != 0) cout << "Podano nieprawidlowe znaki. Sproboj ponownie: " << endl;
                getline(cin, wybor);
            } while (wybor.size() != 1);

            //system("cls");
            switch (wybor[0])
            {
                case '1':
                    if (!avl_binarnie)
                        cout << "Drzewo jest puste" << endl;
                    else
                    {
                        cout << "Sciezka minimum: " << endl;
                        unsigned long long minVar = minAVL(avl_binarnie , false);
                        cout << "Minimum AVL: " << minVar << "\nSciezka maksimum: " << endl;
                        unsigned long long maxVar = maxAVL("" ,avl_binarnie , false);
                        cout << avl_binarnie->val << "\nMaksimum AVL: " << maxVar << endl;
                    }
                    break;
                case '2':
                    if (!avl_binarnie)
                        cout << "Drzewo jest puste" << endl;
                    else
                    {
                        cout << "Podaj liczbe wezlow do usuniecia i nazwy: " << endl;
                        stringstream readStream = stringstream(wprowadzDane());
                        string word;
                        readStream >> word;
                        n = stoull(word);
                        for (unsigned long long i = 0; i != n && avl; ++i)
                        {
                            if (!(readStream >> word)) break;
                            usunAVL_bezrownowagi(stoull(word) , avl_binarnie , avl_binarnie);
                        }
                        rysujAVL("", "" ,avl_binarnie , false);
                    }
                    break;
                case '3':
                    if (!avl_binarnie)
                        cout << "Drzewo jest puste" << endl;
                    else
                    {
                        chrono::time_point<chrono::high_resolution_clock> start = chrono::high_resolution_clock::now();
                        wypiszInOrderAVL(avl_binarnie);
                        if (!test) cout << endl;
                        auto duration = chrono::duration_cast<chrono::microseconds>(chrono::high_resolution_clock::now() - start);
                        if (test)
                            cout << duration.count() << endl;
                    }
                    break;
                case '4':
                    if (!avl_binarnie)
                        cout << "Drzewo jest puste" << endl;
                    else
                    {
                        wypiszPreOrderAVL(avl_binarnie);
                        cout << endl;
                    }
                    break;
                case '5':
                    if (!avl_binarnie)
                        cout << "Drzewo jest puste" << endl;
                    else
                    {
                        karczujAVL(avl_binarnie);
                        cout << "Usuniecie wykonano pomyslnie" << endl;
                        avl = nullptr;
                    }
                    break;
                case '6':
                    if (!avl_binarnie)
                        cout << "Drzewo jest puste" << endl;
                    else
                    {
                        AVL* drzewo = avl_binarnie;
                        cout << "Podaj klucz: ";
                        unsigned long long dane = stoull(wprowadzDane());
                        while (drzewo->val != dane)
                        {
                            if (drzewo->val <= dane)
                                drzewo = drzewo->right;
                            else drzewo = drzewo->left;
                            if (!drzewo) break;
                        }
                        if (!drzewo)
                        {
                            cout << "Nie znaleziono wezla o podanej wartosci" << endl;
                            break;
                        }
                        wypiszPreOrderAVL(drzewo);
                        cout << endl;
                    }
                    break;
                case '8':
                    if (!avl_binarnie)
                        cout << "Drzewo jest puste" << endl;
                    else rysujAVL("" , "" , avl_binarnie , false);
                    break;
                case '9':
                    run = false;
                    break;
                default:
                    goto loop3;
                    break;
            }
        }
    }
    karczujBST();
    karczujAVL();
    karczujAVL(avl_binarnie);
    if (!test) system("pause");
    return 0;
}

bool poprawnoscDanych(stringstream &readStream, unsigned int& n, unsigned long long limit = static_cast<unsigned long long>(-1), string maxVal = maxULL)
{
    n = 0u;
    string word;
    bool bDigit = true;
    while (bDigit && readStream >> word)
    {
        if (word.size() > maxVal.size()) bDigit = false;
        else if (word.size() == maxVal.size() && word > maxVal) bDigit = false;
        else
            for (char c : word)
                if (!isdigit(c))
                    bDigit = false;
        if (n++ == limit) bDigit = false;
    }
    if (n == 0) bDigit = false;
    readStream.clear();
    readStream.seekg(0, ios::beg);
    return bDigit;
}

string wprowadzDane()
{
    string wybor = "", word;
    bool bDigit = true;
    while (wybor.size() == 0 || !bDigit)
    {
        if (!bDigit) cout << "Sproboj ponownie: " << endl;
        getline(cin, wybor);
        stringstream readStream(wybor);
        unsigned int n;
        bDigit = poprawnoscDanych(readStream, n);
    }
    return wybor;
}

bool wczytajDane()
{
    unsigned long long* daneWejsciowe = nullptr;
    unsigned int n = 0u;
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
        if (!poprawnoscDanych(readStream, n))
        {
            cout << "Znaleziono nieprawidlowe dane";    //nie są liczbami bądź zawierają inne znaki niż cyfry
            return false;
        }
        daneWejsciowe = new unsigned long long[n];
        for (int i = 0; i != n; ++i)
        {
            string word;
            readStream >> word;
            daneWejsciowe[i] = stoull(word);
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
            cout << "Podaj ciag liczb naturalnych. Oddzielaj liczby spacja: " << endl;
            bool bDigit = true;
            while (daneWejscioweRaw.size() == 0 || !bDigit)
            {
                if (!bDigit) cout << "Wprowadzony ciag \"" + daneWejscioweRaw + "\" jest "
                    "niepoprawny. Wszystkie wartosci musza byc liczbami naturalnymi <= " +
                    maxULL + " i dlugosc ciagu nie moze przekraczac 10 znakow. Sproboj ponownie: " << endl;
                bDigit = true;
                getline(cin, daneWejscioweRaw);
                stringstream readStream(daneWejscioweRaw);
                bDigit = poprawnoscDanych(readStream, n, 10);
                if (!bDigit) continue;
                daneWejsciowe = new unsigned long long[n];
                for (int i = 0; i != n; ++i)
                {
                    string word;
                    readStream >> word;
                    daneWejsciowe[i] = stoull(word);
                }
            }
            cout << endl;
        }
        else if (wybor == "g")
        {
            mt19937_64 mt_rand(time(0));    //64 bitowy generator liczb pseudolosowych
            uniform_int_distribution<unsigned long long> dane_rand(0ull, 9999ull);
            cout << "Podaj liczbe danych do wygenerowania (n >= 0 <= " + to_string(4000) + "):" << endl;
            bool bDigit = true;
            while (daneWejscioweRaw.size() == 0 || !bDigit)
            {
                if (!bDigit) cout << "Sproboj ponownie: " << endl;
                bDigit = true;
                getline(cin, daneWejscioweRaw);
                stringstream readStream(daneWejscioweRaw);
                bDigit = poprawnoscDanych(readStream, n, 10, maxU);
                if (!bDigit) continue;

                if ((n = stoul(daneWejscioweRaw)) > 4000u) bDigit = false;
            }
            daneWejsciowe = new unsigned long long[n];
            for (int i = 0; i != n; ++i)
                daneWejsciowe[i] = dane_rand(mt_rand);
        }
    }
    if (!daneWejsciowe) return false;
    chrono::time_point<chrono::high_resolution_clock> start = chrono::high_resolution_clock::now();
    stworzBST(daneWejsciowe, n);
    auto duration = chrono::duration_cast<chrono::microseconds>(chrono::high_resolution_clock::now() - start);
    if (test) cout << duration.count() << endl;

    start = chrono::high_resolution_clock::now();
    stworzAVL(daneWejsciowe, n);
    duration = chrono::duration_cast<chrono::microseconds>(chrono::high_resolution_clock::now() - start);
    if (test) cout << duration.count() << endl;

    //AVL BiNaRnIe
    start = chrono::high_resolution_clock::now();
    heapSort(daneWejsciowe - 1, n);
    stworzAVL_bezrownowagi(daneWejsciowe - 1, 1, n, true); // wartosc true -> false jesli trzeba policzyc czas sortowania
    duration = chrono::duration_cast<chrono::microseconds>(chrono::high_resolution_clock::now() - start);
    if (test) cout << duration.count() << endl;

    delete daneWejsciowe;
    return true;
}