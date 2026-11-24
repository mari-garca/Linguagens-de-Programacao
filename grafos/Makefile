grafo: main.o grafo.o aresta.o vertice.o
	g++ -Wall -o grafo main.o grafo.o aresta.o vertice.o

main.o: main.cpp grafo.h aresta.h vertice.h
	g++ -Wall -c main.cpp

grafo.o: grafo.cpp grafo.h aresta.h vertice.h
	g++ -Wall -c grafo.cpp

aresta.o: aresta.cpp aresta.h vertice.h
	g++ -Wall -c aresta.cpp

vertice.o: vertice.cpp vertice.h
	g++ -Wall -c vertice.cpp

clean:
	rm -f *.o grafo
