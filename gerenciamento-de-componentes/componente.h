#ifndef COMPONENTE_H
#define COMPONENTE_H

#include <iostream>
#include <string>

using namespace std;

// struct dos componentes
struct Componente {
    string codigo;
    string fabricante;
    int quantidade;
    double precoUnitario;
};

// operadores do componente
ostream &operator<<(ostream &, const Componente &);
istream &operator>>(istream &, Componente &);
bool operator==(const Componente &, const Componente &);
bool operator<(const Componente &, const Componente &);

// funcao pra converter texto em inteiro
bool converteInteiro(string, int &);

#endif
