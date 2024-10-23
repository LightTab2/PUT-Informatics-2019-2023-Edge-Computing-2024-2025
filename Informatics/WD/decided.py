import random
import numpy as np
                    
materials       = np.genfromtxt("mats.csv",     dtype=None, skip_header = 1, encoding=None)
parts           = np.genfromtxt("parts.csv",    dtype=None, skip_header = 1, encoding=None)
printers        = np.genfromtxt("printers.csv", dtype=None, skip_header = 1, encoding=None)
with open("criteria.csv", "w") as file:
        file.write("MAX cena: ")
        first = True
        for i in range(len(parts)):
                for j in range(len(materials)):
                        if (not first):
                                file.write(" + ")
                        file.write("{:.4f}".format(materials[j][1]/1000.0*parts[i][3]) + "x" + str(i) + "," + str(j))
                        first = False
                        
        file.write("\nMAX precyzja: ")
        first = True
        for i in range(len(parts)):
                for j in range(len(printers)):
                        if (not first):
                                file.write(" + ")
                        file.write("{:.4f}".format(printers[j][-1]*parts[i][-2]) + "z" + str(i) + "," + str(j))
                        first = False

        file.write("\nMIN czasPracy: (")
        first = True
        for i in range(len(parts)):
                for j in range(len(materials)):
                        if (not first):
                                file.write(" + ")
                        file.write("{:.4f}".format(materials[j][-1]/1000.0*parts[i][3]) + "x" + str(i) + "," + str(j))
                        first = False

        file.write(") / (")
        first = True
        for i in range(len(parts)):
                for j in range(len(printers)):
                        if (not first):
                                file.write(" + ")
                        file.write("{:.4f}".format(printers[j][-1]) + "z" + str(i) + "," + str(j))
                        first = False
                        
        file.write(")\nMAX zywotnosc: ")
        first = True
        for i in range(len(parts)):
                for j in range(len(materials)):
                        if (not first):
                                file.write(" + ")
                        file.write(str(materials[j][-2]) + "x" + str(i) + "," + str(j))
                        first = False
