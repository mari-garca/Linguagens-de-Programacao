#include <iomanip>
#include "componente.h"

// imprime um componente (cout << componente)
ostream &operator<<(ostream &saida, const Componente &c) {
    saida << left << setw(14) << c.codigo
          << setw(22) << c.fabricante
          << setw(8) << c.quantidade
          << "R$ " << fixed << setprecision(2) << c.precoUnitario;
    return saida;
}

// le os dados de um componente (cin >> componente)
istream &operator>>(istream &entrada, Componente &c) {
    string texto;

    cout << "Codigo: ";
    if (!getline(entrada, c.codigo))
        return entrada;

    cout << "Fabricante: ";
    if (!getline(entrada, c.fabricante))
        return entrada;

    // quantidade tem que ser inteiro maior que zero
    bool valido = false;
    while (!valido) {
        cout << "Quantidade: ";
        if (!getline(entrada, texto))
            return entrada;
        valido = converteInteiro(texto, c.quantidade) && c.quantidade > 0;
        if (!valido)
            cout << "Quantidade invalida, digite um inteiro maior que zero." << endl;
    }

    // preco tem que ser escrito com ponto
    cout << "Preco unitario (use ponto, ex.: 1.50): ";
    entrada >> c.precoUnitario;
    entrada.ignore(100, '\n');

    return entrada;
}

// dois componentes sao iguais se tem o mesmo codigo
bool operator==(const Componente &a, const Componente &b) {
    return a.codigo == b.codigo;
}

// compara pelo fabricante e depois pelo codigo
bool operator<(const Componente &a, const Componente &b) {
    if (a.fabricante != b.fabricante)
        return a.fabricante < b.fabricante;
    return a.codigo < b.codigo;
}

// converte texto em inteiro
bool converteInteiro(string texto, int &valor) {
    if (texto.size() == 0)
        return false;

    valor = 0;
    for (unsigned i = 0; i < texto.size(); i++) {
        if (texto[i] < '0' || texto[i] > '9')
            return false;
        valor = valor * 10 + (texto[i] - '0');
    }
    return true;
}
