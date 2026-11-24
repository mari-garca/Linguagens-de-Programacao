#include <iostream>
#include <iomanip>
#include <fstream>
#include "grafo.h"

using namespace std;

void imprimeNumero(double valor, int casas) {
    int fator = 1;
    for (int i = 0; i < casas; i++)
        fator = fator * 10;

    int total = static_cast<int>(valor * fator + 0.5);
    cout << total / fator;
    if (casas > 0)
        cout << "," << setfill('0') << setw(casas) << total % fator << setfill(' ');
}

bool converteNumero(string texto, double &valor) {
    double parteInteira = 0;
    double parteDecimal = 0;
    double divisor = 1;
    bool depoisDaVirgula = false;

    if (texto.size() == 0)
        return false;

    for (size_t i = 0; i < texto.size(); i++) {
        char c = texto[i];
        if (c >= '0' && c <= '9') {
            if (depoisDaVirgula) {
                divisor = divisor * 10;
                parteDecimal = parteDecimal + (c - '0') / divisor;
            } else {
                parteInteira = parteInteira * 10 + (c - '0');
            }
        } else if ((c == ',' || c == '.') && !depoisDaVirgula) {
            depoisDaVirgula = true;
        } else {
            return false;
        }
    }

    valor = parteInteira + parteDecimal;
    return true;
}

Grafo::Grafo() {
    numVertices = 0;
    numArestas = 0;
}

int Grafo::getNumVertices() {
    return numVertices;
}

int Grafo::getNumArestas() {
    return numArestas;
}

int Grafo::buscaVertice(string nome) {
    for (int i = 0; i < numVertices; i++)
        if (vertices[i].getRotulo() == nome)
            return i;
    return -1;
}

int Grafo::obtemVertice(string nome) {
    int i = buscaVertice(nome);
    if (i != -1)
        return i;

    vertices[numVertices].setRotulo(nome);
    numVertices++;
    return numVertices - 1;
}

int Grafo::indiceDe(Vertice *v) {
    for (int i = 0; i < numVertices; i++)
        if (&vertices[i] == v)
            return i;
    return -1;
}

int Grafo::grau(int i) {
    int total = 0;
    for (int j = 0; j < numArestas; j++)
        if (arestas[j].temVertice(&vertices[i]))
            total++;
    return total;
}

bool Grafo::insereAresta(string nome1, string nome2, double peso) {
    if (nome1 == nome2) {
        cout << "Aviso: aresta (" << nome1 << ", " << nome2
             << ") ignorada, um vertice nao pode ligar nele mesmo." << endl;
        return false;
    }
    if (peso <= 0) {
        cout << "Aviso: aresta (" << nome1 << ", " << nome2
             << ") ignorada, o peso precisa ser positivo." << endl;
        return false;
    }

    int i1 = buscaVertice(nome1);
    int i2 = buscaVertice(nome2);

    if (i1 != -1 && i2 != -1) {
        for (int j = 0; j < numArestas; j++) {
            if (arestas[j].temVertice(&vertices[i1]) &&
                arestas[j].temVertice(&vertices[i2])) {
                cout << "Aviso: aresta (" << nome1 << ", " << nome2
                     << ") repetida foi ignorada." << endl;
                return false;
            }
        }
    }

    int novos = 0;
    if (i1 == -1) novos++;
    if (i2 == -1) novos++;
    if (numArestas == MAX_ARESTAS || numVertices + novos > MAX_VERTICES) {
        cout << "Aviso: limite de vertices ou arestas atingido." << endl;
        return false;
    }

    i1 = obtemVertice(nome1);
    i2 = obtemVertice(nome2);
    arestas[numArestas] = Aresta(&vertices[i1], &vertices[i2], peso);
    numArestas++;
    return true;
}

bool Grafo::carregaArquivo(string nomeArquivo) {
    fstream arquivo;
    arquivo.open(nomeArquivo.c_str(), fstream::in);

    if (!arquivo.is_open()) {
        cout << "Erro: nao foi possivel abrir o arquivo " << nomeArquivo << endl;
        return false;
    }

    string nome1, nome2, pesoTexto;
    while (arquivo >> nome1 >> nome2 >> pesoTexto) {
        double peso;
        if (converteNumero(pesoTexto, peso))
            insereAresta(nome1, nome2, peso);
        else
            cout << "Aviso: peso invalido na aresta (" << nome1 << ", "
                 << nome2 << "), ela foi ignorada." << endl;
    }

    arquivo.close();
    return true;
}

void Grafo::imprimeListaAdjacencia() {
    cout << "\nLista de adjacencia:" << endl;
    for (int i = 0; i < numVertices; i++) {
        cout << vertices[i].getRotulo() << ":";
        for (int j = 0; j < numArestas; j++) {
            if (arestas[j].temVertice(&vertices[i])) {
                cout << " (" << arestas[j].getVizinho(&vertices[i])->getRotulo() << ", ";
                imprimeNumero(arestas[j].getPeso(), 1);
                cout << ")";
            }
        }
        cout << endl;
    }
}

void Grafo::imprimeGraus() {
    if (numVertices == 0) {
        cout << "\nO grafo esta vazio." << endl;
        return;
    }

    int maior = grau(0);
    int menor = grau(0);
    for (int i = 1; i < numVertices; i++) {
        if (grau(i) > maior) maior = grau(i);
        if (grau(i) < menor) menor = grau(i);
    }

    cout << "\nMaior grau (" << maior << "): ";
    bool primeiro = true;
    for (int i = 0; i < numVertices; i++) {
        if (grau(i) == maior) {
            if (!primeiro) cout << ", ";
            cout << vertices[i].getRotulo();
            primeiro = false;
        }
    }

    cout << "\nMenor grau (" << menor << "): ";
    primeiro = true;
    for (int i = 0; i < numVertices; i++) {
        if (grau(i) == menor) {
            if (!primeiro) cout << ", ";
            cout << vertices[i].getRotulo();
            primeiro = false;
        }
    }

    double medio = 2.0 * numArestas / numVertices;
    cout << "\nGrau medio: ";
    imprimeNumero(medio, 1);
    cout << endl;
}

void Grafo::imprimeComponentesConexas() {
    int componente[MAX_VERTICES];
    int fila[MAX_VERTICES];
    int numComponentes = 0;

    for (int i = 0; i < numVertices; i++)
        componente[i] = -1;

    for (int s = 0; s < numVertices; s++) {
        if (componente[s] != -1)
            continue;

        int inicio = 0, fim = 0;
        fila[fim] = s;
        fim++;
        componente[s] = numComponentes;

        while (inicio < fim) {
            int u = fila[inicio];
            inicio++;

            for (int j = 0; j < numArestas; j++) {
                if (arestas[j].temVertice(&vertices[u])) {
                    int v = indiceDe(arestas[j].getVizinho(&vertices[u]));
                    if (componente[v] == -1) {
                        componente[v] = numComponentes;
                        fila[fim] = v;
                        fim++;
                    }
                }
            }
        }
        numComponentes++;
    }

    cout << "\nNumero de componentes conexas: " << numComponentes << endl;
    for (int c = 0; c < numComponentes; c++) {
        cout << "Componente " << c + 1 << ": {";
        bool primeiro = true;
        for (int i = 0; i < numVertices; i++) {
            if (componente[i] == c) {
                if (!primeiro) cout << ", ";
                cout << vertices[i].getRotulo();
                primeiro = false;
            }
        }
        cout << "}" << endl;
    }
}

void Grafo::imprimeArvoreGeradoraMinima() {
    bool naArvore[MAX_VERTICES];
    bool alcancado[MAX_VERTICES];
    double custo[MAX_VERTICES];
    int arestaEscolhida[MAX_VERTICES];

    for (int i = 0; i < numVertices; i++) {
        naArvore[i] = false;
        alcancado[i] = false;
        custo[i] = 0;
        arestaEscolhida[i] = -1;
    }

    double custoTotal = 0;
    int numEscolhidas = 0;
    int numArvores = 0;

    cout << "\nArvore geradora minima (algoritmo de Prim):" << endl;

    for (int passo = 0; passo < numVertices; passo++) {
        int u = -1;
        for (int i = 0; i < numVertices; i++)
            if (!naArvore[i] && alcancado[i] && (u == -1 || custo[i] < custo[u]))
                u = i;

        if (u == -1) {
            for (int i = 0; i < numVertices && u == -1; i++)
                if (!naArvore[i])
                    u = i;
            numArvores++;
        } else {
            Aresta a = arestas[arestaEscolhida[u]];
            numEscolhidas++;
            cout << "  " << numEscolhidas << ". (" << a.getOrigem()->getRotulo()
                 << ", " << a.getDestino()->getRotulo() << ", ";
            imprimeNumero(a.getPeso(), 1);
            cout << ")" << endl;
            custoTotal = custoTotal + a.getPeso();
        }

        naArvore[u] = true;

        for (int j = 0; j < numArestas; j++) {
            if (arestas[j].temVertice(&vertices[u])) {
                int v = indiceDe(arestas[j].getVizinho(&vertices[u]));
                double peso = arestas[j].getPeso();
                if (!naArvore[v] && (!alcancado[v] || peso < custo[v])) {
                    alcancado[v] = true;
                    custo[v] = peso;
                    arestaEscolhida[v] = j;
                }
            }
        }
    }

    cout << "Custo total: ";
    imprimeNumero(custoTotal, 1);
    cout << endl;

    if (numArvores > 1)
        cout << "Obs: o grafo nao e conexo, entao o resultado e uma floresta com "
             << numArvores << " arvores." << endl;
}

void Grafo::calculaDistancias(int origem, double dist[], bool alcancado[]) {
    bool visitado[MAX_VERTICES];

    for (int i = 0; i < numVertices; i++) {
        dist[i] = 0;
        alcancado[i] = false;
        visitado[i] = false;
    }
    alcancado[origem] = true;

    for (int passo = 0; passo < numVertices; passo++) {
        int u = -1;
        for (int i = 0; i < numVertices; i++)
            if (!visitado[i] && alcancado[i] && (u == -1 || dist[i] < dist[u]))
                u = i;

        if (u == -1)
            return;

        visitado[u] = true;

        for (int j = 0; j < numArestas; j++) {
            if (arestas[j].temVertice(&vertices[u])) {
                int v = indiceDe(arestas[j].getVizinho(&vertices[u]));
                double novaDistancia = dist[u] + arestas[j].getPeso();
                if (!visitado[v] && (!alcancado[v] || novaDistancia < dist[v])) {
                    dist[v] = novaDistancia;
                    alcancado[v] = true;
                }
            }
        }
    }
}

void Grafo::imprimeCentralidadeProximidade() {
    double centralidade[MAX_VERTICES];
    double soma[MAX_VERTICES];
    int alcancaveis[MAX_VERTICES];
    int ordem[MAX_VERTICES];
    double dist[MAX_VERTICES];
    bool alcancado[MAX_VERTICES];

    for (int v = 0; v < numVertices; v++) {
        calculaDistancias(v, dist, alcancado);

        soma[v] = 0;
        alcancaveis[v] = 0;
        for (int t = 0; t < numVertices; t++) {
            if (t != v && alcancado[t]) {
                soma[v] = soma[v] + dist[t];
                alcancaveis[v]++;
            }
        }

        if (soma[v] > 0)
            centralidade[v] = 1.0 / soma[v];
        else
            centralidade[v] = 0;

        ordem[v] = v;
    }

    for (int i = 1; i < numVertices; i++) {
        int atual = ordem[i];
        int j = i - 1;
        while (j >= 0 && centralidade[ordem[j]] < centralidade[atual]) {
            ordem[j + 1] = ordem[j];
            j--;
        }
        ordem[j + 1] = atual;
    }

    cout << "\nVertices em ordem decrescente de centralidade de proximidade:" << endl;
    for (int i = 0; i < numVertices; i++) {
        int v = ordem[i];
        cout << "  " << i + 1 << ". " << vertices[v].getRotulo() << ": c = ";
        imprimeNumero(centralidade[v], 4);
        cout << "  (soma das distancias = ";
        imprimeNumero(soma[v], 1);
        cout << ", " << alcancaveis[v] << " vertices alcancaveis)" << endl;
    }
}
