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

plt.show()
