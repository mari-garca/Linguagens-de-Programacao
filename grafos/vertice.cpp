#include "vertice.h"

Vertice::Vertice(string r) {
    setRotulo(r);
}

void Vertice::setRotulo(string r) {
    rotulo = r;
}

string Vertice::getRotulo() {
    return rotulo;
}
