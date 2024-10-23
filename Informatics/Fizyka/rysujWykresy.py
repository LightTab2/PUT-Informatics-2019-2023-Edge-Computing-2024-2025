from mpl_toolkits.mplot3d import Axes3D
import matplotlib.pyplot as plt
from matplotlib import cm
from matplotlib.ticker import LinearLocator, FormatStrFormatter
import matplotlib.style as mplstyle
import numpy as np
import math
import random

def dodajWykres(Ox, Oy, OyOrg, errory, errorx, label, clr):
    plt.plot(Ox, Oy, label = label, color=clr)
    plt.scatter(Ox, OyOrg, s = 10, color=clr)
    plt.errorbar(Ox, OyOrg, xerr = errorx, yerr=errory, linestyle="None", color=clr)

def dodajWykres2(Ox, Oy, label, clr):
    plt.plot(Ox, Oy, label = label, color=clr)
    
def stworzWykres2d(title, label_x, label_y):
    plt.suptitle(title)
    plt.xlabel(label_x)
    plt.ylabel(label_y)
    plt.legend()
    plt.grid()

T1=[
    26,
    28,
    30,
    33,
    36,
    39,
    42,
    45,
    48,
    51,
    54,
    57,
    60,
    63,
    66,
    69,
    72,
    75,
    78,
    81
]

T2=[
    26,
    29,
    30,
    33,
    36,
    39,
    42,
    45,
    48,
    51,
    54,
    57,
    60,
    63,
    66,
    69,
    72,
    75,
    78,
    81
]     

T3=[
    26,
    29,
    30,
    34,
    36,
    39,
    42,
    45,
    48,
    51,
    54,
    57,
    60,
    63,
    66,
    69,
    72,
    75,
    78,
    81
]     

U1=[
    6.3,
    6.4,
    6.5,
    6.7,
    6.8,
    7,
    7.2,
    7.2,
    7.4,
    7.6,
    7.7,
    8.0,
    8.2,
    8.4,
    8.5,
    8.6,
    8.8,
    8.9,
    9.1,
    9.2
]     

U2=[
    5.4,
    5.4,
    5.5,
    5.6,
    5.7,
    5.8,
    5.9,
    6,
    6.1,
    6.1,
    6.2,
    6.3,
    6.4,
    6.5,
    6.6,
    6.7,
    6.8,
    6.9,
    7,
    7.1
]

U3=[
    4.1,
    4.2,
    4.2,
    4.4,
    4.4,
    4.5,
    4.6,
    4.7,
    4.8,
    4.9,
    5.1,
    5.2,
    5.4,
    5.5,
    5.7,
    5.9,
    6,
    6.2,
    6.3,
    6.4
]   

UBlad = np.array([0.1 for i in range(20)])
TBlad = np.array([1 for i in range(20)])

z1 = np.polyfit(T1, U1, 1)
p1 = np.poly1d(z1)
z2 = np.polyfit(T2, U2, 1)
p2 = np.poly1d(z2)
z3 = np.polyfit(T3, U3, 1)
p3 = np.poly1d(z3)

plt.figure("E")
dodajWykres(T1, p1(T1), U1, UBlad, TBlad, "Termoogniwo A", "r")
dodajWykres(T2, p2(T2), U2, UBlad, TBlad, "Termoogniwo B", "g")
dodajWykres(T3, p3(T3), U3, UBlad, TBlad, "Termoogniwo C", "b")
stworzWykres2d('', 't[°C]', 'U[mV]')
plt.savefig("wykres1.svg")
plt.show()
