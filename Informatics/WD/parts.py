import random
import numpy as np
ILOSC_DANYCH = 20
def ran(array):
        dice = random.randrange(0,100)
        prob = 0
        errorcheck = 0
        for i in array:
                errorcheck += i[1]
        if (errorcheck < 100 or errorcheck > 100):        
                print("Blad: suma prawdopodobienstwa wszystkich zdarzen nie daje 100%")
                print(array)
                return None
        for i in array:
                prob += i[1]
                if (dice < prob):
                        return i[0]
                    
materials = np.genfromtxt("mat.csv", dtype=None, skip_header = 1, encoding=None)

with open("parts.csv", "w") as file:
    file.write("lp. rozmiar_x rozmiar_y ilosc_materialu war_wytrzymalosc war_elastycznosc war_odpornosc_termiczna, war_odpornosc_chemiczna precyzja_priorytet ilosc_zamowien\n")
    for i in range(ILOSC_DANYCH):
        passedAtLeastOne = False
        while (not passedAtLeastOne):
            conditions = (ran([[random.randrange(0,50), 40], [random.randrange(50,75), 40], [random.randrange(75,100), 20]]),
                          ran([[random.randrange(0,30), 45], [random.randrange(30,70), 45], [random.randrange(70,100), 10]]),
                          ran([[random.randrange(0,40), 75], [random.randrange(40,60), 25]]),
                          ran([[random.randrange(0,30), 75], [random.randrange(30,50), 25]]))
            for mat in materials:
                if (passedAtLeastOne):
                    break
                passedAtLeastOne = True
                for j in range(len(conditions)):
                    if (conditions[j] > mat[2+j]):
                        passedAtLeastOne = False
                        break
                    
        size = (ran([[random.randrange(30,50), 70],  [random.randrange(1,30), 30]]),
                ran([[random.randrange(30,50), 70],  [random.randrange(1,30), 30]]))
        materialUse = size[0] * size[1] * random.randrange(80,400)/1000
        file.write(str(i) + " " +
        str(size[0]) + " " +
        str(size[1]) + " " +
        str(materialUse) + " " +
        str(conditions[0]) + " " +
        str(conditions[1]) + " " +
        str(conditions[2]) + " " +
        str(conditions[3]) + " " +
        str(random.randrange(0,500)/100) + " " +
        str(ran([[random.randrange(1,2), 60], [random.randrange(3,10), 40]]))+ "\n"
        )
