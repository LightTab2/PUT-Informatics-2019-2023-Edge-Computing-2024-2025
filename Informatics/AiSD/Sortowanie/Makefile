objects = Main.o

Program.exe: $(objects)
	g++ -O2 -fwhole-program -o "Program.exe" $(objects) 

Main.o: Program.cpp 
	g++ -c -O2 "Program.cpp" -o Main.o

.PHONY: debug
debug:
	g++ -c "Program.cpp" -o Main.o
	g++ -o "Program.exe" $(objects) 

.PHONY : clean
clean :
	-del "Program.exe" $(objects)
