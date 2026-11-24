#ifndef GRAFO_H
#define GRAFO_H

#include <string>
#include "vertice.h"
#include "aresta.h"

using namespace std;

const int MAX_VERTICES = 100;
const int MAX_ARESTAS = 500;

class Grafo {
public:
    Grafo();
    bool carregaArquivo(string);
    bool insereAresta(string, string, double);
    int getNumVertices();
    int getNumArestas();

    void imprimeListaAdjacencia();
    void imprimeGraus();
    void imprimeComponentesConexas();
    void imprimeArvoreGeradoraMinima();
    void imprimeCentralidadeProximidade();

private:
    Vertice vertices[MAX_VERTICES];
    Aresta arestas[MAX_ARESTAS];
    int numVertices;
    int numArestas;

    int buscaVertice(string);
    int obtemVertice(string);
    int indiceDe(Vertice *);
    int grau(int);
    void calculaDistancias(int, double[], bool[]);
};

void imprimeNumero(double, int);
bool converteNumero(string, double &);

#endif
