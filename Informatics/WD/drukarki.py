import random
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

with open("printers.csv", "w") as file:
    file.write("lp. rozmiar_x rozmiar_y wydajnosc precyzja\n")
    for i in range(ILOSC_DANYCH):
        file.write(str(i) + " " +
        str(ran([[30, 20], [40, 20], [50, 20], [random.randrange(30,50), 40]])) + " " +
        str(ran([[30, 20], [40, 20], [50, 20], [random.randrange(30,50), 40]])) + " " +
        str(random.randrange(50,500)/100) + " " + str(random.randrange(20,2500)/100) + "\n"
        )
