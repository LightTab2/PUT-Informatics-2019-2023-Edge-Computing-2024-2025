import matplotlib.pyplot as plt
import matplotlib.style as mplstyle
import numpy as np
import math

def dodajWykres(Ox, Oy, label):
    plt.plot(Ox, Oy, label = label)

def stworzWykres2d(title, label_x, label_y):
    plt.suptitle(title)
    plt.xlabel(label_x)
    plt.ylabel(label_y)
    plt.legend()
    plt.grid()

dane = np.arange(2599, 25000, 1)
y = []
for i in dane:
    y.append(i*math.log(i))
y2 = []
for i in dane:
    y2.append(i*i)
    
plt.figure()
dodajWykres(dane, dane, "f(x)")
stworzWykres2d('Wykres porównawczy f(x) = x', 'Ox', 'Oy')
plt.savefig("Wykresy/Liniowy.svg")
plt.figure()
dodajWykres(dane, y, "f(x)")
stworzWykres2d('Wykres porównawczy f(x) = xlogx', 'Ox', 'Oy')
plt.savefig("Wykresy/LiniowyLog.svg")
plt.figure()
dodajWykres(dane, y2, "f(x)")
stworzWykres2d('Wykres porównawczy f(x) = x^2', 'Ox', 'Oy')
plt.savefig("Wykresy/Kwadartowy.svg")
plt.show()
