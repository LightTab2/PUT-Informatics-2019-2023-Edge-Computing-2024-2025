import numpy as np
import random

def ran(array):
        dice = random.randrange(0,100)
        prob = 0
        errorcheck = 0
        for i in array:
                errorcheck += i[1]
        if (errorcheck < 100 or errorcheck > 100):        
                print("Błąd: suma prawdopodobienstwa wszystkich zdarzeń nie daje 100%")
                print(array)
                return None
        for i in array:
                prob += i[1]
                if (dice < prob):
                        return str(i[0])
         

dane = np.genfromtxt("dane.csv", delimiter = " ", skip_header = 1, dtype = None, encoding = None)
print(dane)
with open("parts.csv", 'w') as out:
    out.write("lp. ilosc warunek_wyt warunek_el warunek_ter warunek_chem\n")
    for i in range(50):
        out.write(str(i) + " " +
                    ran([[100, 10],[150, 15],[200, 50],[250, 10],[300, 15]])+ " " +
                    ran([[0, 30],[random.randrange(1,60), 50],[random.randrange(61,100), 20]])+ " " +
                    ran([[0, 10],[random.randrange(1,60), 50],[random.randrange(61,100), 40]])+ " " +
                    ran([[0, 70],[random.randrange(1,60), 10], [random.randrange(61,100), 20]])+ " " +
                    ran([[0, 50],[random.randrange(1,60), 30], [random.randrange(61,100), 20]])+ "\n")
