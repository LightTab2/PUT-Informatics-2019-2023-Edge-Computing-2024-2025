import sys
import random
import math
from PySide2 import QtWidgets, QtGui, QtCore
import matplotlib.pyplot as plt
#Zmienne globalne
#Skoro proporcja H/L=n_H/n_L=1, to można zapisać, że n_H=n_L i przetrzymać w jednej zmiennej - tutaj nazwanej "rozmiar"
rozmiar = 0
liczba = 0      #liczba atomów w symulacji
#Zmienna "czas" odpowiada za to, po ilu jednostkach czasu zostanie wyświetlony wynik
czas = 0
#Co każdą jednostkę czasu, do zmiennej tick dodawane jest 1
tick = 0
fps = 140       #maksymalna liczba klatek na sekundę i jednocześnie przetwarzanych jednostek czasu na sekundę
R = 596/50      #średnica atomu, we wszystkich obliczeniach jest wkorzystywana średnica, więc przechowywanie promienia i ciągłe mnożenie go przez 2 jest nieoptymalne
#Pozycja atomu w obszarze innego atomu jest wykrywana jako różnica pozycji i sprawdzane jest, czy ta różnica (oznaczę ją jako DIF) wynosi R < DIF < R+d (R+d zapamiętamy w zmiennej Rd, żeby uniknąć ciągłego dodawania w najczęściej używanej funkcji) 
d = R/20
Rd = R+d
V = 1
kappa = 0
droga = 0
#Tablica zderzenia może przechowywać maksymalnie 1000 (max zmiennej 'czas') elementów, które zawierają czas (tick), kiedy wystąpiło zderzenie
zderzenia = []
#2500 atomów to max przy rozmiarze 100
atomy = []
bPaused = False
#newLiczba, tickCounter, result1, result2, wyniki1, wyniki2, bladPomiaru1 i bladPomiaru2 są wykorzystane przy rysowaniu wykresów
newLiczba = 0
tickCounter = [0 for i in range (7)] #M = 10, 20, 50, 100, 200, 500, 1000
#result1, result2 przetrzymuje obecne wyniki symulacji
result1 = [[] for i in range (7)] #Gęstość zderzeń
result2 = [[] for i in range (7)] #Droga
#wyniki1, wyniki2 zachowują średnie z result1 i result2, po tym, gdy zostanie zakończone zbieranie wyników
wyniki1  = [[] for i in range (7)]
wyniki2  = [[] for i in range (7)]
#Bład pomiaru jest liczony z odchylenia standardowego
#bladPomiaru1 = [[] for i in range (7)]
#bladPomiaru2 = [[] for i in range (7)]
#Zrezygnowalismy z zaznaczania błędu pomiaru, bo wykres staje się nieczytelny
#Zmienne, które warto przechować, żeby ich nie liczyć za każdym razem w pętli głównej
scale = 1
minsize = 596
simsize = minsize*rozmiar/100
pointPos = minsize*(100 - rozmiar)/200+2
#Sektory służą do optymalizacji sprawdzania kolizji
liczbaSektorow = 10
liczbaSektorowMinus1 = liczbaSektorow-1
sektory = [[[] for j in range(liczbaSektorow)] for i in range(liczbaSektorow)]
sektoryTreshold = [simsize*j/liczbaSektorow for j in range(liczbaSektorow)]

#Dodaje dane do wykresu
def dodajWykres(Ox, Oy, label):
    plt.plot(Ox, Oy, label = label)

#Tworzy wykres
def stworzWykres2d(title, label_x, label_y):
    plt.suptitle(title)
    plt.xlabel(label_x)
    plt.ylabel(label_y)
    plt.legend()
    plt.grid()

#Klasa od wyświetlania symulacji  
class OpenGLWindow(QtWidgets.QOpenGLWidget):
    def paintEvent(self, event):
        super(OpenGLWindow, self).paintEvent(event)

        #Jak nazwa wskazuje - pozwala rysować na okienku OpenGL
        painter = QtGui.QPainter(self)
        
        #Tło
        painter.setPen(QtGui.QPen(QtGui.QColor(150,120,80),2)) #Pen to obramówka
        painter.setBrush(QtCore.Qt.white) #Brush to wypełnienie

        #Rect zawiera rozmiar i pozycję okna
        Rect = self.geometry()

        #Okienko OpenGL posiada własny system pozycji, u niego wszystko zaczyna się od (0,0), więc przesuwam cały Rect do punktu (2,2) [te 2 pixele to miejsce na obramówkę]
        Rect.moveTopLeft(QtCore.QPoint(2,2))
        #Ustawia nowy rozmiar, który uwzględnia skalę i miejsce na obramówkę
        Rect.setWidth(minsize)
        Rect.setHeight(minsize)
        #Rysuje okno symulacji
        painter.drawRect(Rect)

        #Wyświetlanie ramki
        painter.setPen(QtGui.QPen(QtGui.QColor(255,133,0),2))
        Rect.moveTopLeft(QtCore.QPoint(pointPos, pointPos))
        Rect.setWidth(simsize)
        Rect.setHeight(simsize)
        painter.drawRect(Rect)

        painter.setPen(QtCore.Qt.NoPen)
        painter.setBrush(QtGui.QColor(200,0,0))
        painter.setRenderHint(QtGui.QPainter.Antialiasing)

        #Rysowanie atomów
        if (len(atomy) != 0):
            painter.drawEllipse(pointPos + atomy[0].pos[0], pointPos + atomy[0].pos[1], R, R)
        painter.setBrush(QtGui.QColor(0,0,200))
        for i in range(1,len(atomy)):
            painter.drawEllipse(pointPos + atomy[i].pos[0], pointPos + atomy[i].pos[1], R, R)
        painter.end()

class atom:
    def __init__(self, idnum, position, velocity):
        self.id = idnum
        self.pos = position
        self.vel = velocity
        self.lastHit = []
        self.newHit = []
        
    def checkCollision(self, atom2):
        global zderzenia, droga
        #Nie powinno się zdarzyć, ale lepiej się zabezpieczyć
        if (self.id == atom2.id):
            return
        #Wektor łączący środki atomow będzie wektorem jednostkowym nowej bazy 
        centerConnect = [self.pos[0] - atom2.pos[0], self.pos[1] - atom2.pos[1]]
        length = math.sqrt(centerConnect[0]**2 + centerConnect[1]**2)
        #Treść zadania nakazuje taki warunek na kolizję
        if (R > length) and (length <= Rd):
        #Ale czy nie lepiej?
        #if (length <= Rd):
        #Mamy zabezpieczenia przed podwójnym odbiciem poniżej, więc powyższy warunek gwarantuje, że atomy nie 'przelecą' przez siebie
            if (atom2.id in self.newHit): #optymalizacja pamięci i odciążanie garbage collectora
                return
            self.newHit.append(atom2.id)
                
            if (self.id in atom2.newHit):
                return
            atom2.newHit.append(self.id)
                
            if (atom2.id in self.lastHit):
                return
            
            if (self.id == 0):
                zderzenia.append([tick, droga])
                droga = 0
                
            #W nowej bazie pierwszą współrzędną wektora prędkości stanowi prędkość w kierunku zderzenia, a y --- prostopadła do niej
            base = [[centerConnect[0], centerConnect[1]], [centerConnect[1], -centerConnect[0]]]
            
            #Wyznacznik jest potrzebny do szybkiego obliczenia macierzy odwrotnej, dzięki której prędkość będzie można przekształcić na prędkość w nowej bazie 
            det = 1.0/(base[0][0]*base[1][1] - base[0][1]*base[1][0])
            baseInv = [[det*base[1][1], -det*base[0][1]], [-det*base[1][0], det*base[0][0]]]

            vel1 = [self.vel[0] * baseInv[0][0] + self.vel[1] * baseInv[0][1],
                    self.vel[0] * baseInv[1][0] + self.vel[1] * baseInv[1][1]]
            vel2 = [atom2.vel[0] * baseInv[0][0] + atom2.vel[1] * baseInv[0][1],
                    atom2.vel[0] * baseInv[1][0] + atom2.vel[1] * baseInv[1][1]]
            
            #Magia zderzenia polega na zamianie współrzędnych x wektorów vel1 i vel2, a potem przekształcenie z powrotem do naszej pierwotnej bazy
            self.vel = [vel2[0]*base[0][0] + vel1[1]*base[0][1],
                        vel2[0]*base[1][0] + vel1[1]*base[1][1]]
            atom2.vel = [vel1[0]*base[0][0] + vel2[1]*base[0][1],
                        vel1[0]*base[1][0] + vel2[1]*base[1][1]]
    
    def checkCollisionWall(self):
        if (self.pos[0] <= d) and (self.vel[0] < 0):
            self.vel[0] = -self.vel[0]
            self.newHit.clear()
        elif (self.pos[0] >= simsize - Rd) and (self.vel[0] > 0):
            self.vel[0] = -self.vel[0]
            self.newHit.clear()
        if (self.pos[1] <= d) and (self.vel[1] < 0):
            self.vel[1] = -self.vel[1]
            self.newHit.clear()
        elif (self.pos[1] >= simsize - Rd) and (self.vel[1] > 0):
            self.vel[1] = -self.vel[1]   
            self.newHit.clear()

class main(QtWidgets.QWidget):
    def mainLoop(self):
        global atomy, kappa, tick, zderzenia, droga
        if (bPaused):
            self.OpenGLWidget.update()
            return

        #Sprawdzanie kolizji, naiwnie
        #for i in range(liczba):
            #for j in range(i+1, liczba):
                #atomy[i].checkCollision(atomy[j])

        #Sprawdzanie kolizji zoptymalizowane
        self.checkSectors()
        for i in range(liczbaSektorow):
            for j in range(liczbaSektorow):
                for k in range(len(sektory[i][j])):
                    for l in range(k+1, len(sektory[i][j])):
                        sektory[i][j][k].checkCollision(sektory[i][j][l])
        
        for i in range(liczba):
            atomy[i].checkCollisionWall()
            atomy[i].pos[0] += atomy[i].vel[0]/kappa#(V*kappa)
            atomy[i].pos[1] += atomy[i].vel[1]/kappa#(V*kappa)
            atomy[i].lastHit = atomy[i].newHit[:]
            atomy[i].newHit.clear()
        if (liczba != 0):
            droga += math.sqrt(atomy[0].vel[0]**2 + atomy[0].vel[1]**2)/kappa#(V*kappa)
        
        self.OpenGLWidget.update()      #Wyświetlanie

        #Jeżeli czas zderzenia odbył się ponad maksymalny czas temu, to nie ma potrzeby go przechowywać w tablicy i można go usunąć
        if len(zderzenia) and (tick - zderzenia[0][0] == 1001):
            del zderzenia[0]
        self.calculateResult()
        #self.createPlots()
        tick += 1

    #Poniżej znajduje się sprawdzanie przynależności atomów do sektorów
    #Sektor to część symulacji (podzieliliśmy ją na [liczbaSektorów]^2 części)
    #Kolizje są sprawdzane pomiędzy atomami w sektorach
    #Sprawdzanie kolizji pomiędzy atomami ma złożoność O(n^2)
    #Ten algorytm nie poprawia klasy złożoności
    #Jedynie sprawia, że jest wykonywanych potencjalnie mniej operacji
    #Bo zamienia liczbę operacji z c_1n^2 na c_2m^2, gdzie c_2 > c_1, ale m < n, co zazwyczaj się opłaca
    #Kod wydaje się skomplikowany, ale nie jest
    #Wystarczy dobrze zrozumieć oznaczenia
    def checkSectors(self):
        global sektory
        sektory = [[[] for j in range(liczbaSektorow)] for i in range(liczbaSektorow)]
        for i in range(liczba):
            x = atomy[i].pos[0] - Rd
            xR = x + 2*Rd
            y = atomy[i].pos[1] - Rd
            yR = y + 2*Rd
                
            for sx in range(liczbaSektorowMinus1):
                if (xR > sektoryTreshold[sx]) and (x < sektoryTreshold[sx+1]):
                    for sy in range(liczbaSektorowMinus1):
                         if (yR > sektoryTreshold[sy]) and (y < sektoryTreshold[sy+1]):
                            sektory[sx][sy].append(atomy[i])
                    if (yR > sektoryTreshold[liczbaSektorowMinus1]):
                        sektory[sx][liczbaSektorowMinus1].append(atomy[i])
                        
            if (xR > sektoryTreshold[liczbaSektorowMinus1]):
                for sy in range(liczbaSektorowMinus1):
                    if (yR > sektoryTreshold[sy]) and (y < sektoryTreshold[sy+1]):
                        sektory[liczbaSektorowMinus1][sy].append(atomy[i])
                
            if (xR > sektoryTreshold[liczbaSektorowMinus1]) and (yR > sektoryTreshold[liczbaSektorowMinus1]):
                sektory[liczbaSektorowMinus1][liczbaSektorowMinus1].append(atomy[i])
                
    def calculateResult(self):
        #Gdy jeszcze nie minęło wystarczająco dużo czasu, zamiast liczby wypisane zostanie '-'
        if (tick < czas):
            self.zderzeniaValLabel.setText("-")
            self.drogaValLabel.setText("-")
            return

        zderzeniaVal = 0
        drogaVal = 0.0
        #Oblicza drogę oraz gęstość zderzeń
        for x in range(len(zderzenia)-1, -1, -1):
            if (tick - zderzenia[x][0] > czas):
                break
            zderzeniaVal += 1
            drogaVal += zderzenia[x][1]
        self.zderzeniaValLabel.setText(str(zderzeniaVal/czas))
        if (zderzeniaVal != 0):
            drogaVal /= zderzeniaVal
        self.drogaValLabel.setText(str(round(drogaVal)))

    def createPlots(self):
        global tick, newLiczba, result1, result2, wyniki1, wyniki2, tickCounter
        #Dajemy trochę czasu, żeby atomy, które powstały w kolidujących ze sobą miejscach, przestały ze sobą kolidować ,,od wewnątrz''
        if (tick < 1000) or (tick > 5000):
            return
        
        self.calculateResultForPlot(0, 10)
        self.calculateResultForPlot(1, 20)
        self.calculateResultForPlot(2, 50)
        self.calculateResultForPlot(3, 100)
        self.calculateResultForPlot(4, 200)
        self.calculateResultForPlot(5, 500)
        self.calculateResultForPlot(6, 1000)
            
        if (tick == 5000):
            result1_avg = [0 for i in range(7)]
            result2_avg = [0 for i in range(7)]
            for i in range(7):
                avg1 = 0.0
                avg2 = 0.0
                for x in range(len(result1[i])):
                    avg1 += result1[i][x]
                    
                for x in range(len(result2[i])):
                    avg2 += result2[i][x]
                    
                avg1 /= len(result1[i])
                avg2 /= len(result2[i])
                result1_avg[i] = avg1
                result2_avg[i] = avg2
            
            #Oblicza błąd pomiaru z odchylenia standardowego
            #for i in range(7):
                #for x in range(len(result1[i])):
                    #result1_err[i] += (result1[i][x]-result1_avg[i])**2
                #for x in range(len(result2[i])):
                    #result2_err[i] += (result2[i][x]-result2_avg[i])**2  
                #result1_err[i] = math.sqrt(result1_err[i]/len(result1[i]))
                #result2_err[i] = math.sqrt(result2_err[i]/len(result2[i]))
                
            for i in range(7):
                if (len(wyniki1[i]) < 2):
                    wyniki1[i].append(0)
                    #bladPomiaru1[i].append(0)
                else:
                    wyniki1[i].append(result1_avg[i])
                    #bladPomiaru1[i].append(result1_err[i])

                if (len(wyniki2[i]) < 2):
                    wyniki2[i].append(0)
                    #bladPomiaru2[i].append(0)
                else:
                    wyniki2[i].append(result2_avg[i])
                    #bladPomiaru2[i].append(result2_err[i])

            end = 101
            if (newLiczba == end-1):
                fig = plt.figure("1")
                dodajWykres([i for i in range(end)], wyniki1[0], "M = 10")
                dodajWykres([i for i in range(end)], wyniki1[1], "M = 20")
                dodajWykres([i for i in range(end)], wyniki1[2], "M = 50")
                dodajWykres([i for i in range(end)], wyniki1[3], "M = 100")
                dodajWykres([i for i in range(end)], wyniki1[4], "M = 200")
                dodajWykres([i for i in range(end)], wyniki1[5], "M = 500")
                dodajWykres([i for i in range(end)], wyniki1[6], "M = 1000")
                stworzWykres2d('n(N)', 'Liczba atomów', 'Gęstość zderzeń')
                fig.savefig("1.svg")
                                                     
                fig = plt.figure("2")
                dodajWykres([i for i in range(end)], wyniki2[0], "M = 10")
                dodajWykres([i for i in range(end)], wyniki2[1], "M = 20")
                dodajWykres([i for i in range(end)], wyniki2[2], "M = 50")
                dodajWykres([i for i in range(end)], wyniki2[3], "M = 100")
                dodajWykres([i for i in range(end)], wyniki2[4], "M = 200")
                dodajWykres([i for i in range(end)], wyniki2[5], "M = 500")
                dodajWykres([i for i in range(end)], wyniki2[6], "M = 1000")
                stworzWykres2d('λ(N)', 'Liczba atomów', 'Droga')
                fig.savefig("2.svg")
                plt.show()
            else:
                print(newLiczba)
                newLiczba += 1
                self.changeLiczba(newLiczba)
                tick = 0
                zderzenia.clear()
            result1 = [[] for i in range (7)]
            result2 = [[] for i in range (7)]
            tickCounter = [0 for i in range (7)]
            
    def calculateResultForPlot(self, index, tickTime):
        global result1, result2, tickCounter
        if ((tick - tickCounter[index]) < tickTime):
            return
        zderzeniaVal = 0
        drogaVal = 0.0
        for x in range(len(zderzenia)-1, -1, -1):
            if (tick - zderzenia[x][0] > tickTime):
                break
            zderzeniaVal += 1
            drogaVal += zderzenia[x][1]
            
        if (zderzeniaVal != 0):
            drogaVal /= zderzeniaVal
        result1[index].append(zderzeniaVal/tickTime)
        result2[index].append(drogaVal)
        tickCounter[index] = tick
                
    #Poniżej znajdują się funkcje wywoływane przy naciśnięciu przycisków/przesuwaniu suwaków oraz funkcja inicjalizacji
    def changeLiczba(self, arg):
        global liczba, atomy
        if (type(arg) is str):
            if (len(arg) == 0):
                arg = 0
            arg = int(arg)
            
        self.liczbaSpin.setDisabled(True)
        self.liczbaSpin.setValue(arg)
        self.liczbaSpin.setDisabled(False)
            
        if (arg > liczba):
            start = len(atomy)
            if (start == 0):
                atomy = [atom(0, [0,0], [random.uniform(0,V), random.uniform(0,V)])]
                for i in range(1, arg - start):
                    atomy.append(atom(i, [random.uniform(0, simsize-R), random.uniform(0, simsize-R)], [random.uniform(-V,V), random.uniform(-V,V)]))
            else:
                for i in range(arg - liczba):
                    atomy.append(atom(start+i, [random.uniform(0, simsize-R), random.uniform(0, simsize-R)], [random.uniform(-V,V), random.uniform(-V,V)]))
                
        elif (arg < liczba):
            for i in range(0, liczba - arg):
                if (len(atomy) != 0):
                    atomy.pop()
        liczba = arg
            
    def changeRozmiar(self, arg):
        global rozmiar, atomy, liczba, simsize, pointPos, sektoryTreshold
        if (type(arg) is str):
            if (len(arg) == 0):
                arg = 20
            arg = int(arg)
            self.rozmiarSlider.setDisabled(True)
            self.rozmiarSlider.setValue(arg)
            self.rozmiarSlider.setDisabled(False)
            
        if (arg < 20):
            arg = 20
            self.rozmiarSlider.setDisabled(True)
            self.rozmiarSlider.setValue(arg)
            self.rozmiarSlider.setDisabled(False)
        elif (arg > 100):
            arg = 100
            
        self.rozmiarText.setText(str(arg))
        #W treści zadania jest, że N może mieć conajwyżej 1/4 pojemnosci 
        newLiczba = math.floor((arg**2)/4)
        if (newLiczba < liczba):
            self.changeLiczba(newLiczba)
            
        self.liczbaSpin.setMaximum(newLiczba)
        if (liczba != 0):        
            posscale = arg/rozmiar
            for i in range(0,len(atomy)):
                atomy[i].pos[0] *= posscale
                atomy[i].pos[1] *= posscale
            
        minsize = min(self.OpenGLWidget.geometry().width(), self.OpenGLWidget.geometry().height()) - 4
        simsize = minsize*rozmiar/100
        pointPos = minsize*(100 - rozmiar)/200+2
        sektoryTreshold = [simsize/liczbaSektorow*j for j in range(liczbaSektorow)]
        rozmiar = arg

    def changeCzas(self, arg):
        global czas
        if (type(arg) is str):
            if (len(arg) == 0):
                arg = 10
            arg = int(arg)
            self.czasSlider.setDisabled(True)
            self.czasSlider.setValue(arg)
            self.czasSlider.setDisabled(False)
        else:
            arg = round(arg/10) * 10
 
        if (arg < 10):
            arg = 10
        if (arg > 1000):
            arg = 1000
            
        self.czasText.blockSignals(True)
        self.czasText.setText(str(arg))
        self.czasText.blockSignals(False)
        czas = arg
        self.calculateResult()

    def changeD(self, arg):
        global d
        if (type(arg) is str):
            if (len(arg) == 0):
                arg = 10
            arg = int(arg)
            self.dSlider.setDisabled(True)
            self.dSlider.setValue(arg)
            self.dSlider.setDisabled(False)
            
        if (arg < 10):
            arg = 10
        if (arg > 100):
            arg = 100
            
        self.dText.blockSignals(True)
        self.dText.setText(str(arg))
        self.dText.blockSignals(False)
        
        #W ten sposób zawsze d <= R/10
        d = R * arg/1000
        
    def changeKappa(self, arg):
        global kappa
        if (type(arg) is str):
            if (len(arg) == 0):
                arg = 10
            arg = int(arg)
            self.kappaSlider.setDisabled(True)
            self.kappaSlider.setValue(arg)
            self.kappaSlider.setDisabled(False)
            
        if (arg < 10):
            arg = 10
        if (arg > 100):
            arg = 100
            
        self.kappaText.blockSignals(True)
        self.kappaText.setText(str(arg))
        self.kappaText.blockSignals(False)

        kappa = arg/100

    def reset(self):
        global atomy, zderzenia, tick
        liczbaKopia = liczba
        self.changeLiczba(0)
        self.changeLiczba(liczbaKopia)
        zderzenia.clear()
        tick = 0

    def pause(self):
        global bPaused
        bPaused = not bPaused
        if (bPaused):
            self.pauseButton.setText("Wznów symulację")
        else:
            self.pauseButton.setText("Pauza symulacji")

    def default(self):
        self.changeRozmiar("70")
        self.changeLiczba("100")
        self.changeCzas("500")
        self.changeD("50")
        self.changeKappa("50")
        
    #Inicjalizacja. Tworzy całe GUI i łączy z innymi funkcjami
    def __init__(self):
        super(main, self).__init__()
        
        #Okno
        self.setWindowTitle("Projekt 1.")
        
        #Użytkownik nie może zmienić rozmiaru okna poniżej 800x600
        self.setMinimumSize(800,600)
        x
        #Domyślne rozmiary okienka
        self.resize(800,600)
        
        #Opisy tekstowe, drugim argumentem jest rodzic. Jeżeli go nie nadamy, te elementy się nie wyświetlą w naszym oknie (powstałyby nowe okna dla każdego obiektu QtWidget)
        self.liczbaLabel = QtWidgets.QLabel("Liczba:", self)
        self.rozmiarLabel = QtWidgets.QLabel("Rozmiar:", self)
        self.czasLabel = QtWidgets.QLabel("Czas:", self)
        self.wynikiLabel = QtWidgets.QLabel("Wyniki:", self)
        self.wynikiLabel.setAlignment(QtCore.Qt.AlignCenter)
        self.drogaLabel = QtWidgets.QLabel("Droga:", self)
        self.drogaValLabel = QtWidgets.QLabel("-", self)
        self.zderzeniaLabel = QtWidgets.QLabel("Gęstość zderzeń:", self)
        self.zderzeniaValLabel = QtWidgets.QLabel("-", self)
        self.advLabel = QtWidgets.QLabel("Opcje zaawansowane:", self)
        self.advLabel.setAlignment(QtCore.Qt.AlignCenter)
        self.dLabel = QtWidgets.QLabel("d:", self)
        self.kappaLabel = QtWidgets.QLabel("κ:", self)
        self.rozmiarText = QtWidgets.QLineEdit("-", self)
        self.czasText = QtWidgets.QLineEdit("-", self)
        self.dText = QtWidgets.QLineEdit("-", self)
        self.kappaText = QtWidgets.QLineEdit("-", self)

        #Pokrętła
        self.liczbaSpin = QtWidgets.QSpinBox(self)
        self.liczbaSpin.setMaximum((70**2)/4)
        #Tak wygląda łączenie sygnału QtWidget z funkcją
        self.liczbaSpin.valueChanged.connect(self.changeLiczba)

        #Okno symulacji
        self.OpenGLWidget = OpenGLWindow(self)
        self.OpenGLWidget.initializeGL()
        
        #Suwaki
        self.rozmiarSlider = QtWidgets.QSlider(QtCore.Qt.Orientation.Horizontal, self)
        self.rozmiarSlider.setMaximum(100)
        self.rozmiarSlider.valueChanged.connect(self.changeRozmiar)

        self.czasSlider = QtWidgets.QSlider(QtCore.Qt.Orientation.Horizontal, self)
        self.czasSlider.setMaximum(1000)
        self.czasSlider.valueChanged.connect(self.changeCzas)
        #Najmniejszy możliwy ruch suwaka przesuwa o 10
        self.czasSlider.setSingleStep(10)
        
        self.dSlider = QtWidgets.QSlider(QtCore.Qt.Orientation.Horizontal, self)
        self.dSlider.setMaximum(100)
        self.dSlider.setMinimum(10)
        self.dSlider.valueChanged.connect(self.changeD)
        
        self.kappaSlider = QtWidgets.QSlider(QtCore.Qt.Orientation.Horizontal, self)
        self.kappaSlider.setMaximum(100)
        self.kappaSlider.setMinimum(10)
        self.kappaSlider.valueChanged.connect(self.changeKappa)

        #Pola tekstowe
        self.rozmiarText.setValidator(QtGui.QIntValidator(20, 100, self))
        self.rozmiarText.textChanged.connect(self.changeRozmiar)

        self.czasText.setValidator(QtGui.QIntValidator(0, 1000, self))
        self.czasText.textChanged.connect(self.changeCzas)
        self.dText.setValidator(QtGui.QIntValidator(0, 100, self))
        self.dText.textChanged.connect(self.changeD)
        self.kappaText.setValidator(QtGui.QIntValidator(10, 100, self))
        self.kappaText.textChanged.connect(self.changeKappa)
        
        #Przyciski
        self.defaultButton = QtWidgets.QPushButton("Domyślne ustawienia", self)
        self.defaultButton.clicked.connect(self.default)
        self.pauseButton = QtWidgets.QPushButton("Pauza symulacji", self)
        self.pauseButton.clicked.connect(self.pause)
        self.restartButton = QtWidgets.QPushButton("Restart symulacji", self)
        self.restartButton.clicked.connect(self.reset)

        #Antialiasing dla okna
        formatGL = QtGui.QSurfaceFormat();
        formatGL.setSamples(8);
        self.OpenGLWidget.setFormat(formatGL);

        #Na koniec funkcja rescale() tak naprawdę ustawia wszystkie elementy na swoje pozycje
        self.rescale()
        #Upewnia się, że żadne pole tekstowe nie jest zaznaczone
        self.setFocus()
        #Ustawia domyślne opcje
        self.default()
        #Wywołuje pętlę główną
        self.timer = QtCore.QTimer(self)
        self.timer.timeout.connect(self.mainLoop)
        self.timer.start(1000/fps)
        
    def resizeEvent(self, evt=None):
        self.rescale()

    #Funckja od skalowania obrazu, po powiększeniu okna przez użytkownika
    def rescale(self):
        global scale, minsize, simsize, pointPos, sektoryTreshold, R, Rd
        guiscale = self.size().height()/600
        #Opisy tekstowe
        self.liczbaLabel.setGeometry(10, round(20*guiscale),130,20)
        self.rozmiarLabel.setGeometry(10,round(60*guiscale),130,20)
        self.czasLabel.setGeometry(10,round(110*guiscale),130,20)
        self.wynikiLabel.setGeometry(10,round(170*guiscale),130,20)
        self.drogaLabel.setGeometry(10,round(200*guiscale),130,20)
        self.drogaValLabel.setGeometry(60,round(200*guiscale),130,20)
        self.zderzeniaLabel.setGeometry(10,round(230*guiscale),130,20)
        self.zderzeniaValLabel.setGeometry(110, round(230*guiscale),130,20)
        self.advLabel.setGeometry(10,round(450*guiscale),130,20)
        self.dLabel.setGeometry(10,round(490*guiscale),130,20)
        self.kappaLabel.setGeometry(10,round(540*guiscale),130,20)
    
        self.rozmiarText.setGeometry(100,round(60*guiscale),40,20)
        self.czasText.setGeometry(100,round(110*guiscale),40,20)
        self.dText.setGeometry(100,round(490*guiscale),40,20)
        self.kappaText.setGeometry(100,round(540*guiscale),40,20)
        
        #Pokrętła
        self.liczbaSpin.setGeometry(100,round(20*guiscale),40,20)

        #Suwaki
        self.rozmiarSlider.setGeometry(10,round(80*guiscale),130,20)
        self.czasSlider.setGeometry(10,round(130*guiscale),130,20)
        self.dSlider.setGeometry(10,round(515*guiscale),130,20)
        self.kappaSlider.setGeometry(10,round(565*guiscale),130,20)

        #Przyciski
        self.defaultButton.setGeometry(10,round(280*guiscale),130,20)
        self.pauseButton.setGeometry(10,round(310*guiscale),130,20)
        self.restartButton.setGeometry(10,round(340*guiscale),130,20)

        #Okno symulacji
        self.OpenGLWidget.setGeometry(150,0,self.size().width() - 150, self.size().height())
        minsize = min(self.OpenGLWidget.geometry().width(), self.OpenGLWidget.geometry().height()) - 4
        newscale = minsize/596
        for i in range(len(atomy)):
            atomy[i].pos[0] *= newscale/scale
            atomy[i].pos[1] *= newscale/scale
            atomy[i].vel[0] *= newscale/scale
            atomy[i].vel[1] *= newscale/scale
        R *= newscale/scale
        Rd *= newscale/scale
        scale = newscale
        simsize = minsize*rozmiar/100
        pointPos = minsize*(100 - rozmiar)/200+2
        sektoryTreshold = [simsize/liczbaSektorow*i for i in range(liczbaSektorow)]
        
#Tworzy aplikację
app = QtWidgets.QApplication([])
widget = main()
widget.show()
sys.exit(app.exec_())
