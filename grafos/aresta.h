#ifndef ARESTA_H
#define ARESTA_H

#include "vertice.h"

class Aresta {
public:
    Aresta(Vertice * = 0, Vertice * = 0, double = 1.0);
    Vertice *getOrigem();
    Vertice *getDestino();
    double getPeso();
    bool temVertice(Vertice *);
    Vertice *getVizinho(Vertice *);

private:
    Vertice *origem;
    Vertice *destino;
    double peso;
};

#endif
