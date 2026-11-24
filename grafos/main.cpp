#include <iostream>
#include <string>
#include "grafo.h"

using namespace std;

void exibeMenu() {
    cout << "\n========== MENU ==========" << endl;
    cout << "1 - Imprimir lista de adjacencia" << endl;
    cout << "2 - Maior grau, menor grau e grau medio" << endl;
    cout << "3 - Componentes conexas" << endl;
    cout << "4 - Arvore geradora minima" << endl;
    cout << "5 - Centralidade de proximidade" << endl;
    cout << "0 - Sair" << endl;
    cout << "Opcao: ";
}

int main() {
    string nomeArquivo;
    cout << "Digite o nome do arquivo de entrada (Enter para usar colegas.txt): ";
    getline(cin, nomeArquivo);
    if (nomeArquivo == "")
        nomeArquivo = "colegas.txt";

    Grafo grafo;
    if (!grafo.carregaArquivo(nomeArquivo))
        return 1;

    if (grafo.getNumVertices() == 0) {
        cout << "Erro: nenhuma aresta valida no arquivo." << endl;
        return 1;
    }

    cout << "Grafo carregado: " << grafo.getNumVertices() << " vertices e "
         << grafo.getNumArestas() << " arestas." << endl;

    string opcao;
    while (true) {
        exibeMenu();
        if (!getline(cin, opcao))
            break;

        if (opcao == "0") {
            cout << "Saindo..." << endl;
            break;
        } else if (opcao == "1") {
            grafo.imprimeListaAdjacencia();
        } else if (opcao == "2") {
            grafo.imprimeGraus();
        } else if (opcao == "3") {
            grafo.imprimeComponentesConexas();
        } else if (opcao == "4") {
            grafo.imprimeArvoreGeradoraMinima();
        } else if (opcao == "5") {
            grafo.imprimeCentralidadeProximidade();
        } else {
            cout << "Opcao invalida, digite um numero de 0 a 5." << endl;
        }
    }

    return 0;
}
