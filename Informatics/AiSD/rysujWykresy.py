from mpl_toolkits.mplot3d import Axes3D
import matplotlib.pyplot as plt
from matplotlib import cm
from matplotlib.ticker import LinearLocator, FormatStrFormatter
import matplotlib.style as mplstyle
import numpy as np
import math
import random

def dodajWykres(Ox, Oy, error, label):
    plt.plot(Ox, Oy, label = label)

def stworzWykres2d(title, label_x, label_y):
    plt.suptitle(title)
    plt.xlabel(label_x)
    plt.ylabel(label_y)
    plt.legend()
    plt.grid()

U = []
I = []
plik = open('schemat1.txt', 'r')
plik.readline()
while (True):
    words = plik.readline().split()
    print(words)
    if (len(words) == 0):
        break
    U.append(float(words[1]))
    I.append(float(words[2]))
plik.close()
dodajWykres(U, I , 0, "UGS <-5..5>")

U = []
I = []
plik = open('schemat2.txt', 'r')
plik.readline()
while (True):
    words = plik.readline().split()
    print(words)
    if (len(words) == 0):
        break
    U.append(float(words[1]))
    I.append(float(words[2]))
plik.close()
dodajWykres(U, I , 0 , "UGS <-7,5..2,5>")
stworzWykres2d('I(UDS)', 'UDS [V]', 'I [A]')
plt.savefig('wyk1.svg')
plt.show()
