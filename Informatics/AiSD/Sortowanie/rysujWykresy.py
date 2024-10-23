import matplotlib.pyplot as plt
import matplotlib.style as mplstyle
import numpy as np

def dodajWykres(Ox, Oy, label):
    plt.plot(Ox, Oy, label = label)

def stworzWykres2d(title, label_x, label_y):
    plt.suptitle(title)
    plt.xlabel(label_x)
    plt.ylabel(label_y)
    plt.legend()
    plt.grid()

def dodajWykresIDane(pliknazwa, etykieta):
    czas_merge = []
    operacje_merge = []
    czas_heap = []
    operacje_heap = []
    operacje2_heap = []
    czas_insertion = []
    operacje_insertion = []
    operacje2_insertion = []
    czas_shell = []
    operacje_shell = []
    operacje2_shell = []
    czas_quick = []
    operacje_quick = []
    operacje2_quick = []

    for i in range(15):
        plik = open("Wyniki/" + pliknazwa + "_0_" + str(i), 'r')
        czas_merge.append(int(plik.readline()))
        operacje_merge.append(int(plik.readline()))
        czas_heap.append(int(plik.readline()))
        operacje_heap.append(int(plik.readline()))
        operacje2_heap.append(int(plik.readline()))
        czas_insertion.append(int(plik.readline()))
        operacje_insertion.append(int(plik.readline()))
        operacje2_insertion.append(int(plik.readline()))
        czas_shell.append(int(plik.readline()))
        operacje_shell.append(int(plik.readline()))
        operacje2_shell.append(int(plik.readline()))
        czas_quick.append(int(plik.readline()))
        operacje_quick.append(int(plik.readline()))
        operacje2_quick.append(int(plik.readline()))
        plik.close()

    for p in range(1, 5):
        for i in range(15):
            plik = open("Wyniki/" + pliknazwa + "_" + str(p) + "_" + str(i), 'r')
            czas_merge[i] += int(plik.readline())
            operacje_merge[i] += int(plik.readline())
            czas_heap [i] += int(plik.readline())     
            operacje_heap[i] += int(plik.readline())
            operacje2_heap[i] += int(plik.readline())
            czas_insertion[i] += int(plik.readline())
            operacje_insertion[i] += int(plik.readline())
            operacje2_insertion[i] += int(plik.readline())
            czas_shell[i] += int(plik.readline())
            operacje_shell[i] += int(plik.readline())
            operacje2_shell[i] += int(plik.readline())
            czas_quick[i] += int(plik.readline())
            operacje_quick[i] += int(plik.readline())
            operacje2_quick[i] += int(plik.readline())
            plik.close()

    for i in range(15):
        czas_merge[i] /= 5
        operacje_merge[i] /= 5
        czas_heap[i] /= 5
        operacje_heap[i] /= 5
        operacje2_heap[i] /= 5
        czas_insertion[i] /= 5
        operacje_insertion[i] /= 5
        operacje2_insertion[i] /= 5
        czas_shell[i] /= 5
        operacje_shell[i] /= 5
        operacje2_shell[i] /= 5
        czas_quick[i] /= 5
        operacje_quick[i] /= 5
        operacje2_quick[i] /= 5
        
    plt.figure("merge")
    dodajWykres(dane, czas_merge, etykieta)
    plt.figure("mergeo")
    dodajWykres(dane, operacje_merge, etykieta + " - porównania")
    plt.figure("heap")
    dodajWykres(dane, czas_heap, etykieta)
    plt.figure("heapo")
    dodajWykres(dane, operacje_heap, etykieta + " - porównania")
    dodajWykres(dane, operacje2_heap, etykieta + " - podstawienia")
    plt.figure("insertion")
    dodajWykres(dane, czas_insertion, etykieta)
    plt.figure("insertiono")
    dodajWykres(dane, operacje_insertion, etykieta + " - porównania")
    dodajWykres(dane, operacje2_insertion, etykieta + " - podstawienia")
    plt.figure("shell")
    dodajWykres(dane, czas_shell, etykieta)
    plt.figure("shello")
    dodajWykres(dane, operacje_shell, etykieta + " - porównania")
    dodajWykres(dane, operacje2_shell, etykieta + " - podstawienia")
    plt.figure("quick")
    dodajWykres(dane, czas_quick, etykieta)
    plt.figure("quicko")
    dodajWykres(dane, operacje_quick, etykieta + " - porównania")
    dodajWykres(dane, operacje2_quick, etykieta + " - podstawienia")
    plt.figure(pliknazwa)
    dodajWykres(dane, czas_merge, "Merge Sort")
    dodajWykres(dane, czas_heap, "Heap Sort")
    dodajWykres(dane, czas_insertion, "Insertion Sort")
    dodajWykres(dane, czas_shell, "Shell Sort")
    dodajWykres(dane, czas_quick, "Quick Sort")
    stworzWykres2d(etykieta + ' - zależność czasu od rozmiaru danych', 'Rozmiar danych', 'Czas (μs)')
    plt.savefig("Wykresy/" + pliknazwa + ".svg")


plt.figure("merge")
plt.figure("heap")
plt.figure("heapo")
plt.figure("insertion")
plt.figure("insertiono")
plt.figure("shell")
plt.figure("shello")
plt.figure("quick")
plt.figure("quicko")
plt.figure("ran")
plt.figure("ros")
plt.figure("mal")
plt.figure("A")
plt.figure("V")
dane = np.arange(2599, 25000, 1600)

dodajWykresIDane("ran", "Dane losowe")
dodajWykresIDane("ros", "Dane rosnące")
dodajWykresIDane("mal", "Dane malejące")
dodajWykresIDane("A", "Dane A-kształtne")
dodajWykresIDane("V", "Dane V-kształtne")

plt.figure("merge")
stworzWykres2d('Merge Sort - czas', 'Rozmiar danych', 'Czas (μs)')
plt.savefig("Wykresy/merge.svg")
plt.figure("mergeo")
stworzWykres2d('Merge Sort - operacje', 'Rozmiar danych', 'Operacje')
plt.savefig("Wykresy/mergeo.svg")
plt.figure("heap")
stworzWykres2d('Heap Sort - czas', 'Rozmiar danych', 'Czas (μs)')
plt.savefig("Wykresy/heap.svg")
plt.figure("heapo")
stworzWykres2d('Heap Sort - operacje', 'Rozmiar danych', 'Operacje')
plt.savefig("Wykresy/heapo.svg")
plt.figure("insertion")
stworzWykres2d('Insertion Sort - czas', 'Rozmiar danych', 'Czas (μs)')
plt.savefig("Wykresy/insertion.svg")
plt.figure("insertiono")
stworzWykres2d('Insertion Sort - operacje', 'Rozmiar danych', 'Operacje')
plt.savefig("Wykresy/insertiono.svg")
plt.figure("shell")
stworzWykres2d('Shell Sort - czas', 'Rozmiar danych', 'Czas (μs)')
plt.savefig("Wykresy/shell.svg")
plt.figure("shello")
stworzWykres2d('Shell Sort - operacje', 'Rozmiar danych', 'Operacje')
plt.savefig("Wykresy/shello.svg")
plt.figure("quick")
stworzWykres2d('Quick Sort - czas', 'Rozmiar danych', 'Czas (μs)')
plt.savefig("Wykresy/quick.svg")
plt.figure("quicko")
stworzWykres2d('Quick Sort - operacje', 'Rozmiar danych', 'Operacje')
plt.savefig("Wykresy/quicko.svg")
plt.show()
