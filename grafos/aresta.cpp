#include "aresta.h"

Aresta::Aresta(Vertice *o, Vertice *d, double p) {
    origem = o;
    destino = d;
    peso = p;
}

Vertice *Aresta::getOrigem() {
    return origem;
}

Vertice *Aresta::getDestino() {
    return destino;
}

double Aresta::getPeso() {
    return peso;
}

bool Aresta::temVertice(Vertice *v) {
    return v == origem || v == destino;
}

Vertice *Aresta::getVizinho(Vertice *v) {
    if (v == origem)
        return destino;
    return origem;
}
