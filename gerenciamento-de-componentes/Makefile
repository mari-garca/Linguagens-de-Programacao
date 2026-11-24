estoque: main.o componente.o estoque.o
	g++ -Wall -o estoque main.o componente.o estoque.o

main.o: main.cpp componente.h estoque.h
	g++ -Wall -c main.cpp

componente.o: componente.cpp componente.h
	g++ -Wall -c componente.cpp

estoque.o: estoque.cpp estoque.h componente.h
	g++ -Wall -c estoque.cpp

clean:
	rm -f *.o estoque
