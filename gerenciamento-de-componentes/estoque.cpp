#include <fstream>
#include <iomanip>
#include "estoque.h"

// construtor: carrega o estoque do arquivo
EstoqueComponentes::EstoqueComponentes(int cap, string arquivo) {
    capacidade = cap;
    nomeArquivo = arquivo;
    persistente = true;
    carregaArquivo();
}

// construtor de copia: a copia nunca salva o arquivo
EstoqueComponentes::EstoqueComponentes(const EstoqueComponentes &outro) {
    componentes = outro.componentes;
    capacidade = outro.capacidade;
    nomeArquivo = outro.nomeArquivo;
    persistente = false;
}

// destrutor: salva o arquivo antes de terminar
EstoqueComponentes::~EstoqueComponentes() {
    if (persistente)
        salvaArquivo();
}

// atribuicao: copia os componentes mas mantem o persistente
const EstoqueComponentes &EstoqueComponentes::operator=(const EstoqueComponentes &outro) {
    if (&outro != this) {
        componentes = outro.componentes;
        capacidade = outro.capacidade;
    }
    return *this;
}

// gets
int EstoqueComponentes::getCapacidade() const {
    return capacidade;
}

int EstoqueComponentes::getNumComponentes() const {
    return static_cast<int>(componentes.size());
}

// procura o componente pelo codigo e devolve a posicao (-1 se nao achar)
int EstoqueComponentes::buscaPosicao(const Componente &c) const {
    for (int i = 0; i < getNumComponentes(); i++)
        if (componentes[i] == c)
            return i;
    return -1;
}

// busca pelo codigo (usado no menu)
bool EstoqueComponentes::buscaComponente(string codigo, Componente &resultado) const {
    Componente chave;
    chave.codigo = codigo;
    chave.quantidade = 0;
    chave.precoUnitario = 0;

    int pos = buscaPosicao(chave);
    if (pos == -1)
        return false;
    resultado = componentes[pos];
    return true;
}

// insere na posicao criando um vector com um lugar a mais
void EstoqueComponentes::insereNaPosicao(int pos, const Componente &c) {
    vector<Componente> novo(componentes.size() + 1);
    int j = 0;
    for (int i = 0; i < static_cast<int>(novo.size()); i++) {
        if (i == pos) {
            novo[i] = c;
        } else {
            novo[i] = componentes[j];
            j++;
        }
    }
    componentes = novo;
}

// remove da posicao criando um vector com um lugar a menos
void EstoqueComponentes::removeDaPosicao(int pos) {
    vector<Componente> novo(componentes.size() - 1);
    int j = 0;
    for (int i = 0; i < getNumComponentes(); i++) {
        if (i != pos) {
            novo[j] = componentes[i];
            j++;
        }
    }
    componentes = novo;
}

// entrada de componente (estoque += componente)
EstoqueComponentes &EstoqueComponentes::operator+=(const Componente &c) {
    int pos = buscaPosicao(c);

    // ja existe: soma a quantidade e atualiza o preco
    if (pos != -1) {
        componentes[pos].quantidade += c.quantidade;
        componentes[pos].precoUnitario = c.precoUnitario;
        return *this;
    }

    // estoque cheio: nao muda nada
    if (getNumComponentes() >= capacidade)
        return *this;

    // procura a posicao certa pra manter a ordem
    int destino = 0;
    while (destino < getNumComponentes() && componentes[destino] < c)
        destino++;
    insereNaPosicao(destino, c);
    return *this;
}

// retirada de componente (estoque -= componente)
EstoqueComponentes &EstoqueComponentes::operator-=(const Componente &c) {
    int pos = buscaPosicao(c);

    if (pos == -1 || c.quantidade <= 0 || c.quantidade > componentes[pos].quantidade)
        return *this;

    // retira e se zerar remove do vector
    componentes[pos].quantidade -= c.quantidade;
    if (componentes[pos].quantidade == 0)
        removeDaPosicao(pos);
    return *this;
}

// reajuste de precos: devolve uma copia com os precos novos
EstoqueComponentes EstoqueComponentes::operator*(double fator) const {
    EstoqueComponentes novo(*this);
    for (int i = 0; i < novo.getNumComponentes(); i++)
        novo.componentes[i].precoUnitario = novo.componentes[i].precoUnitario * fator;
    return novo;
}

// valor total do estoque (static_cast<double>(estoque))
EstoqueComponentes::operator double() const {
    double total = 0;
    for (int i = 0; i < getNumComponentes(); i++)
        total = total + componentes[i].quantidade * componentes[i].precoUnitario;
    return total;
}

// consulta por faixa de preco (inclui o minimo e exclui o maximo)
vector<Componente> EstoqueComponentes::operator()(double minimo, double maximo) const {
    int encontrados = 0;
    for (int i = 0; i < getNumComponentes(); i++)
        if (componentes[i].precoUnitario >= minimo && componentes[i].precoUnitario < maximo)
            encontrados++;

    vector<Componente> resultado(encontrados);
    int j = 0;
    for (int i = 0; i < getNumComponentes(); i++) {
        if (componentes[i].precoUnitario >= minimo && componentes[i].precoUnitario < maximo) {
            resultado[j] = componentes[i];
            j++;
        }
    }
    return resultado;
}

// imprime o estoque completo (cout << estoque)
ostream &operator<<(ostream &saida, const EstoqueComponentes &estoque) {
    if (estoque.getNumComponentes() == 0) {
        saida << "Estoque vazio." << endl;
    } else {
        saida << left << setw(14) << "Codigo" << setw(22) << "Fabricante"
              << setw(8) << "Qtd" << "Preco unit." << endl;
        saida << "------------------------------------------------------------" << endl;
        for (int i = 0; i < estoque.getNumComponentes(); i++)
            saida << estoque.componentes[i] << endl;
    }
    saida << "Componentes distintos: " << estoque.getNumComponentes()
          << " de " << estoque.capacidade << endl;
    return saida;
}

// le o arquivo e coloca os componentes no estoque
void EstoqueComponentes::carregaArquivo() {
    fstream arquivo;
    arquivo.open(nomeArquivo.c_str(), fstream::in);

    if (!arquivo.is_open()) {
        cout << "Arquivo " << nomeArquivo
             << " nao encontrado, o estoque comeca vazio." << endl;
        return;
    }

    // cada linha: codigo;fabricante;quantidade;preco
    string codigo;
    while (getline(arquivo, codigo, ';')) {
        Componente c;
        c.codigo = codigo;
        getline(arquivo, c.fabricante, ';');
        arquivo >> c.quantidade;
        arquivo.ignore(1);
        arquivo >> c.precoUnitario;
        arquivo.ignore(100, '\n');

        if (buscaPosicao(c) == -1 && getNumComponentes() >= capacidade) {
            cout << "Aviso: estoque cheio, o componente " << c.codigo
                 << " do arquivo foi ignorado." << endl;
            continue;
        }

        *this += c;
    }

    arquivo.close();
}

// reescreve o arquivo inteiro
void EstoqueComponentes::salvaArquivo() const {
    fstream arquivo;
    arquivo.open(nomeArquivo.c_str(), fstream::out);

    if (!arquivo.is_open()) {
        cout << "Erro: nao foi possivel salvar o arquivo " << nomeArquivo << endl;
        return;
    }

    arquivo << setprecision(10);
    for (int i = 0; i < getNumComponentes(); i++) {
        arquivo << componentes[i].codigo << ";" << componentes[i].fabricante << ";"
                << componentes[i].quantidade << ";" << componentes[i].precoUnitario << endl;
    }

    arquivo.close();
    cout << "Estoque salvo em " << nomeArquivo << "." << endl;
}
