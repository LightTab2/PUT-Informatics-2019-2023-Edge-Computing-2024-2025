objects = Main.o

Generator.exe: $(objects)
	g++ -O3 -fwhole-program -D DNDEBUG -o "Generator.exe" $(objects) 

Main.o: Generator.cpp 
	g++ -c -O3 "Generator.cpp" -o Main.o

.PHONY: debug
debug:
	g++ -c "Generator.cpp" -o Main.o
	g++ -o "Generator.exe" $(objects) 

.PHONY : clean
clean :
	-del "Generator.exe" $(objects)
