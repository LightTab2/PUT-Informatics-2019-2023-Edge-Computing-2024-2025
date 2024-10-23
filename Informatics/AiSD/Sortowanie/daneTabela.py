dane = [ 2599, 4199, 5799, 7399, 8999, 10599, 12199, 13799, 15399,
         16999, 18599, 20199, 21799, 23399, 24999]

def wypiszDoPliku(nazwaPliku, label, tabela):
    plik = open("Tabele\\" + nazwaPliku, "w", encoding="utf-8")
    plik.write("\\begin{tabular}{|c|c|c|c|c|c|}\n\\hline\n")
    plik.write("\\multicolumn{6}{|c|}{" + label + "}\\\\\n\\hline\n")
    plik.write("$n$ & P 1 & P 2 & P 3 & P 4 & P 5 \\\\\n\\hline\n")
    for i in range(15):
        plik.write(str(dane[i]) + " & ")
        for p in range(5):
            plik.write(str(tabela[p][i]))
            if (p == 4):
                plik.write(" \\\\\n")
            else:
                plik.write(" & ")
        plik.write("\\hline\n")
    plik.write("\\end{tabular}")
    plik.close()

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

def dodajWykresIDane(pliknazwa):
    global czas_merge
    global operacje_merge
    global czas_heap
    global operacje_heap
    global operacje2_heap
    global czas_insertion
    global operacje_insertion
    global operacje2_insertion
    global czas_shell
    global operacje_shell
    global operacje2_shell
    global czas_quick
    global operacje_quick
    global operacje2_quick

    czas_merge.clear()
    operacje_merge.clear()
    czas_heap.clear()
    operacje_heap.clear()
    operacje2_heap.clear()
    czas_insertion.clear()
    operacje_insertion.clear()
    operacje2_insertion.clear()
    czas_shell.clear()
    operacje_shell.clear()
    operacje2_shell.clear()
    czas_quick.clear()
    operacje_quick.clear()
    operacje2_quick.clear()
    for i in range(5):
        czas_merge.append([])
        operacje_merge.append([])
        czas_heap.append([])
        operacje_heap.append([])
        operacje2_heap.append([])
        czas_insertion.append([])
        operacje_insertion.append([])
        operacje2_insertion.append([])
        czas_shell.append([])
        operacje_shell.append([])
        operacje2_shell.append([])
        czas_quick.append([])
        operacje_quick.append([])
        operacje2_quick.append([])
        
    for p in range(5):
        for i in range(15):
            plik = open("Wyniki/" + pliknazwa + "_" + str(p) + "_" + str(i), 'r')
            czas_merge[p].append(int(plik.readline()))
            operacje_merge[p].append(int(plik.readline()))
            czas_heap[p].append(int(plik.readline()))
            operacje_heap[p].append(int(plik.readline()))
            operacje2_heap[p].append(int(plik.readline()))
            czas_insertion[p].append(int(plik.readline()))
            operacje_insertion[p].append(int(plik.readline()))
            operacje2_insertion[p].append(int(plik.readline()))
            czas_shell[p].append(int(plik.readline()))
            operacje_shell[p].append(int(plik.readline()))
            operacje2_shell[p].append(int(plik.readline()))
            czas_quick[p].append(int(plik.readline()))
            operacje_quick[p].append(int(plik.readline()))
            operacje2_quick[p].append(int(plik.readline()))
            plik.close()
            
dodajWykresIDane("ran")
wypiszDoPliku("merge_czas_ran", "Czas Merge Sort - dane losowe", czas_merge)
wypiszDoPliku("merge_operacje_ran", "Operacje porównań Merge Sort - dane losowe", operacje_merge)
wypiszDoPliku("heap_czas_ran", "Czas Heap Sort - dane losowe", czas_heap)
wypiszDoPliku("heap_operacje_ran", "Operacje porównań Heap Sort - dane losowe", operacje_heap)
wypiszDoPliku("heap_operacje2_ran", "Operacje podstawień Heap Sort - dane losowe", operacje2_heap)
wypiszDoPliku("insertion_czas_ran", "Czas Insertion Sort - dane losowe", czas_insertion)
wypiszDoPliku("insertion_operacje_ran", "Operacje porównań Insertion Sort - dane losowe", operacje_insertion)
wypiszDoPliku("insertion_operacje2_ran", "Operacje podstawień Insertion Sort - dane losowe", operacje2_insertion)
wypiszDoPliku("shell_czas_ran", "Czas Shell Sort - dane losowe", czas_shell)
wypiszDoPliku("shell_operacje_ran", "Operacje porównań Shell Sort - dane losowe", operacje_shell)
wypiszDoPliku("shell_operacje2_ran", "Operacje podstawień Shell Sort - dane losowe", operacje2_shell)
wypiszDoPliku("quick_czas_ran", "Czas Quick Sort - dane losowe", czas_quick)
wypiszDoPliku("quick_operacje_ran", "Operacje porównań Quick Sort - dane losowe", operacje_quick)
wypiszDoPliku("quick_operacje2_ran", "Operacje podstawień Quick Sort - dane losowe", operacje2_quick)

dodajWykresIDane("ros")
wypiszDoPliku("merge_czas_ros", "Czas Merge Sort - dane rosnące", czas_merge)
wypiszDoPliku("merge_operacje_ros", "Operacje porównań Merge Sort - dane rosnące", operacje_merge)
wypiszDoPliku("heap_czas_ros", "Czas Heap Sort - dane rosnące", czas_heap)
wypiszDoPliku("heap_operacje_ros", "Operacje porównań Heap Sort - dane rosnące", operacje_heap)
wypiszDoPliku("heap_operacje2_ros", "Operacje podstawień Heap Sort - dane rosnące", operacje2_heap)
wypiszDoPliku("insertion_czas_ros", "Czas Insertion Sort - dane rosnące", czas_insertion)
wypiszDoPliku("insertion_operacje_ros", "Operacje porównań Insertion Sort - dane rosnące", operacje_insertion)
wypiszDoPliku("insertion_operacje2_ros", "Operacje podstawień Insertion Sort - dane rosnące", operacje2_insertion)
wypiszDoPliku("shell_czas_ros", "Czas Shell Sort - dane rosnące", czas_shell)
wypiszDoPliku("shell_operacje_ros", "Operacje porównań Shell Sort - dane rosnące", operacje_shell)
wypiszDoPliku("shell_operacje2_ros", "Operacje podstawień Shell Sort - dane rosnące", operacje2_shell)
wypiszDoPliku("quick_czas_ros", "Czas Quick Sort - dane rosnące", czas_quick)
wypiszDoPliku("quick_operacje_ros", "Operacje porównań Quick Sort - dane rosnące", operacje_quick)
wypiszDoPliku("quick_operacje2_ros", "Operacje podstawień Quick Sort - dane rosnące", operacje2_quick)

dodajWykresIDane("mal")
wypiszDoPliku("merge_czas_mal", "Czas Merge Sort - dane malejące", czas_merge)
wypiszDoPliku("merge_operacje_mal", "Operacje porównań Merge Sort - dane malejące", operacje_merge)
wypiszDoPliku("heap_czas_mal", "Czas Heap Sort - dane malejące", czas_heap)
wypiszDoPliku("heap_operacje_mal", "Operacje porównań Heap Sort - dane malejące", operacje_heap)
wypiszDoPliku("heap_operacje2_mal", "Operacje podstawień Heap Sort - dane malejące", operacje2_heap)
wypiszDoPliku("insertion_czas_mal", "Czas Insertion Sort - dane malejące", czas_insertion)
wypiszDoPliku("insertion_operacje_mal", "Operacje porównań Insertion Sort - dane malejące", operacje_insertion)
wypiszDoPliku("insertion_operacje2_mal", "Operacje podstawień Insertion Sort - dane malejące", operacje2_insertion)
wypiszDoPliku("shell_czas_mal", "Czas Shell Sort - dane malejące", czas_shell)
wypiszDoPliku("shell_operacje_mal", "Operacje porównań Shell Sort - dane malejące", operacje_shell)
wypiszDoPliku("shell_operacje2_mal", "Operacje podstawień Shell Sort - dane malejące", operacje2_shell)
wypiszDoPliku("quick_czas_mal", "Czas Quick Sort - dane malejące", czas_quick)
wypiszDoPliku("quick_operacje_mal", "Operacje porównań Quick Sort - dane malejące", operacje_quick)
wypiszDoPliku("quick_operacje2_mal", "Operacje podstawień Quick Sort - dane malejące", operacje2_quick)

dodajWykresIDane("A")
wypiszDoPliku("merge_czas_A", "Czas Merge Sort - dane A-kształtne", czas_merge)
wypiszDoPliku("merge_operacje_A", "Operacje porównań Merge Sort - dane A-kształtne", operacje_merge)
wypiszDoPliku("heap_czas_A", "Czas Heap Sort - dane A-kształtne", czas_heap)
wypiszDoPliku("heap_operacje_A", "Operacje porównań Heap Sort - dane A-kształtne", operacje_heap)
wypiszDoPliku("heap_operacje2_A", "Operacje podstawień Heap Sort - dane A-kształtne", operacje2_heap)
wypiszDoPliku("insertion_czas_A", "Czas Insertion Sort - dane A-kształtne", czas_insertion)
wypiszDoPliku("insertion_operacje_A", "Operacje porównań Insertion Sort - dane A-kształtne", operacje_insertion)
wypiszDoPliku("insertion_operacje2_A", "Operacje podstawień Insertion Sort - dane A-kształtne", operacje2_insertion)
wypiszDoPliku("shell_czas_A", "Czas Shell Sort - dane A-kształtne", czas_shell)
wypiszDoPliku("shell_operacje_A", "Operacje porównań Shell Sort - dane A-kształtne", operacje_shell)
wypiszDoPliku("shell_operacje2_A", "Operacje podstawień Shell Sort - dane A-kształtne", operacje2_shell)
wypiszDoPliku("quick_czas_A", "Czas Quick Sort - dane A-kształtne", czas_quick)
wypiszDoPliku("quick_operacje_A", "Operacje porównań Quick Sort - dane A-kształtne", operacje_quick)
wypiszDoPliku("quick_operacje2_A", "Operacje podstawień Quick Sort - dane A-kształtne", operacje2_quick)

dodajWykresIDane("V")
wypiszDoPliku("merge_czas_V", "Czas Merge Sort - dane V-kształtne", czas_merge)
wypiszDoPliku("merge_operacje_V", "Operacje porównań Merge Sort - dane V-kształtne", operacje_merge)
wypiszDoPliku("heap_czas_V", "Czas Heap Sort - dane V-kształtne", czas_heap)
wypiszDoPliku("heap_operacje_V", "Operacje porównań Heap Sort - dane V-kształtne", operacje_heap)
wypiszDoPliku("heap_operacje2_V", "Operacje podstawień Heap Sort - dane V-kształtne", operacje2_heap)
wypiszDoPliku("insertion_czas_V", "Czas Insertion Sort - dane V-kształtne", czas_insertion)
wypiszDoPliku("insertion_operacje_V", "Operacje porównań Insertion Sort - dane V-kształtne", operacje_insertion)
wypiszDoPliku("insertion_operacje2_V", "Operacje podstawień Insertion Sort - dane V-kształtne", operacje2_insertion)
wypiszDoPliku("shell_czas_V", "Czas Shell Sort - dane V-kształtne", czas_shell)
wypiszDoPliku("shell_operacje_V", "Operacje porównań Shell Sort - dane V-kształtne", operacje_shell)
wypiszDoPliku("shell_operacje2_V", "Operacje podstawień Shell Sort - dane V-kształtne", operacje2_shell)
wypiszDoPliku("quick_czas_V", "Czas Quick Sort - dane V-kształtne", czas_quick)
wypiszDoPliku("quick_operacje_V", "Operacje porównań Quick Sort - dane V-kształtne", operacje_quick)
wypiszDoPliku("quick_operacje2_V", "Operacje podstawień Quick Sort - dane V-kształtne", operacje2_quick)
