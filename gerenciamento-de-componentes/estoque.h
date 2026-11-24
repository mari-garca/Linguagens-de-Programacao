#ifndef ESTOQUE_H
#define ESTOQUE_H

#include <iostream>
#include <string>
#include <vector>
#include "componente.h"

using namespace std;

// classe para o estoque de componentes
class EstoqueComponentes {
    friend ostream &operator<<(ostream &, const EstoqueComponentes &);

public:
    // construtores, destrutor e atribuicao
    EstoqueComponentes(int, string = "estoque.txt");
    EstoqueComponentes(const EstoqueComponentes &);
    ~EstoqueComponentes();
    const EstoqueComponentes &operator=(const EstoqueComponentes &);

    // operadores do estoque
    EstoqueComponentes &operator+=(const Componente &);
    EstoqueComponentes &operator-=(const Componente &);
    EstoqueComponentes operator*(double) const;
    operator double() const;
    vector<Componente> operator()(double, double) const;

    // gets e busca
    int getCapacidade() const;
    int getNumComponentes() const;
    bool buscaComponente(string, Componente &) const;

private:
    // atributos
    vector<Componente> componentes;
    int capacidade;
    string nomeArquivo;
    bool persistente;

    // funcoes de uso interno
    int buscaPosicao(const Componente &) const;
    void insereNaPosicao(int, const Componente &);
    void removeDaPosicao(int);
    void carregaArquivo();
    void salvaArquivo() const;
};

#endif
