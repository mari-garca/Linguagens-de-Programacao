#include <iostream>
#include <iomanip>
#include <string>
#include <vector>
#include "componente.h"
#include "estoque.h"

using namespace std;

// capacidade maxima do estoque
const int CAPACIDADE_MAXIMA = 20;

// menu
void exibeMenu() {
    cout << "\n=============== MENU ===============" << endl;
    cout << "1 - Mostrar estoque completo" << endl;
    cout << "2 - Entrada de componente" << endl;
    cout << "3 - Retirada de componente" << endl;
    cout << "4 - Consultar componente pelo codigo" << endl;
    cout << "5 - Consultar por faixa de preco" << endl;
    cout << "6 - Reajuste de precos" << endl;
    cout << "7 - Valor total do estoque" << endl;
    cout << "0 - Sair" << endl;
    cout << "Opcao: ";
}

// funcoes pra ler do teclado
bool leTexto(string mensagem, string &texto) {
    cout << mensagem;
    return static_cast<bool>(getline(cin, texto));
}

bool leInteiroPositivo(string mensagem, int &valor) {
    string texto;
    while (true) {
        if (!leTexto(mensagem, texto))
            return false;
        if (converteInteiro(texto, valor) && valor > 0)
            return true;
        cout << "Valor invalido, digite um inteiro maior que zero." << endl;
    }
}

bool leReal(string mensagem, double &valor) {
    cout << mensagem;
    cin >> valor;
    cin.ignore(100, '\n');
    return static_cast<bool>(cin);
}

int main() {
    // cria o estoque (ja carrega o arquivo)
    EstoqueComponentes estoque(CAPACIDADE_MAXIMA, "estoque.txt");
    cout << "Estoque carregado com " << estoque.getNumComponentes()
         << " componentes (capacidade maxima: " << estoque.getCapacidade() << ")." << endl;

    string opcao;
    bool continuar = true;

    while (continuar) {
        exibeMenu();
        if (!getline(cin, opcao))
            break;
        cout << endl;

        if (opcao == "0") { // sair
            continuar = false;

        } else if (opcao == "1") { // mostrar estoque
            cout << estoque;

        } else if (opcao == "2") { // entrada
            Componente c;
            cin >> c;
            if (!cin)
                break;

            Componente existente;
            bool jaExiste = estoque.buscaComponente(c.codigo, existente);
            bool cheio = estoque.getNumComponentes() >= estoque.getCapacidade();

            estoque += c;

            if (jaExiste)
                cout << "Componente ja existia: quantidade somada e preco atualizado." << endl;
            else if (cheio)
                cout << "Estoque cheio! O componente nao foi cadastrado." << endl;
            else
                cout << "Componente cadastrado." << endl;

        } else if (opcao == "3") { // retirada
            Componente retirada;
            retirada.fabricante = "";
            retirada.precoUnitario = 0;
            if (!leTexto("Codigo: ", retirada.codigo))
                break;
            if (!leInteiroPositivo("Quantidade a retirar: ", retirada.quantidade))
                break;

            Componente existente;
            if (!estoque.buscaComponente(retirada.codigo, existente)) {
                cout << "Componente nao encontrado, o estoque nao foi alterado." << endl;
            } else if (retirada.quantidade > existente.quantidade) {
                cout << "Quantidade insuficiente (disponivel: " << existente.quantidade
                     << "), o estoque nao foi alterado." << endl;
            } else {
                estoque -= retirada;
                int restante = existente.quantidade - retirada.quantidade;
                if (restante == 0)
                    cout << "Retirada feita. O estoque zerou e o componente foi removido." << endl;
                else
                    cout << "Retirada feita. Restam " << restante << " unidades." << endl;
            }

        } else if (opcao == "4") { // consultar pelo codigo
            string codigo;
            if (!leTexto("Codigo: ", codigo))
                break;

            Componente c;
            if (estoque.buscaComponente(codigo, c))
                cout << c << endl;
            else
                cout << "Componente nao encontrado." << endl;

        } else if (opcao == "5") { // faixa de preco
            double minimo, maximo;
            if (!leReal("Preco minimo, incluido (use ponto, ex.: 0.50): ", minimo))
                break;
            if (!leReal("Preco maximo, nao incluido (use ponto, ex.: 5.00): ", maximo))
                break;

            vector<Componente> resultado = estoque(minimo, maximo);
            if (resultado.size() == 0) {
                cout << "Nenhum componente nessa faixa de preco." << endl;
            } else {
                cout << resultado.size() << " componente(s) encontrado(s):" << endl;
                for (unsigned i = 0; i < resultado.size(); i++)
                    cout << resultado[i] << endl;
            }

        } else if (opcao == "6") { // reajuste
            double fator;
            if (!leReal("Fator de reajuste (use ponto, ex.: 1.10 para +10%): ", fator))
                break;
            if (fator <= 0) {
                cout << "O fator precisa ser maior que zero." << endl;
                continue;
            }

            EstoqueComponentes reajustado = estoque * fator;
            cout << "Estoque com os precos reajustados:" << endl << reajustado;
            cout << "Valor total depois do reajuste: R$ " << fixed << setprecision(2)
                 << static_cast<double>(reajustado) << endl;

            string resposta;
            if (!leTexto("Aplicar o reajuste no estoque? (s/n): ", resposta))
                break;
            if (resposta == "s" || resposta == "S") {
                estoque = reajustado;
                cout << "Reajuste aplicado." << endl;
            } else {
                cout << "Reajuste descartado, o estoque nao foi alterado." << endl;
            }

        } else if (opcao == "7") { // valor total
            double total = static_cast<double>(estoque);
            cout << "Valor total do estoque: R$ " << fixed << setprecision(2) << total << endl;

        } else {
            cout << "Opcao invalida, digite um numero de 0 a 7." << endl;
        }
    }

    cout << "Saindo..." << endl;
    return 0;
}
