#include <iostream>

using namespace std;

const int MAXINT = 2147483647; // "plus nieskoñczonoœæ"

int main()
{
    int d[100][100];                  // Macierz minimalnych kosztów dojœcia
    int i, j, k, n, m, x, y, w;
    cout << "Podaj liczbe wierzcholkow i krawedzi" << endl;
    cin >> n >> m;             // Czytamy liczbê wierzcho³ków oraz krawêdzi

    for (i = 0; i <= n; i++)
    {
        for (j = 0; j <= n; j++)

            if (i != j) {
                d[i][j] = MAXINT;
            }
            else d[i][j] = 0;
    }

    for (i = 0; i < m; i++)
    {
        cout << "Podaj numery: od do waga ";
        cin >> x >> y >> w;      // Czytamy definicjê krawêdzi
        d[x][y] = w;        // Wagê krawêdzi umieszczamy w macierzy d
    }

    // Algorytm Floyda-Warshalla

    for (k = 0; k <= n; k++)
    {
        for (i = 0; i <= n; i++)
            for (j = 0; j <= n; j++)
            {
                if ((d[i][k] == MAXINT) || (d[k][j] == MAXINT)) continue;
                w = d[i][k] + d[k][j];
                if (d[i][j] > w) d[i][j] = w;
            }

        for (int p = 1; p <= n; p++)
        {
            for (int h = 1; h <= n; h++)
            {
                cout << " d [ " << p << ", " << h << " ] = ";
                if (d[p][h] == MAXINT) { cout << "N "; }
                else { cout << d[p][h]; }
            }
            cout << endl;
        }
        cout << endl;

    }
    // Wyœwietlamy wyniki



    return 0;
}
