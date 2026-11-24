#include <iostream>
#include <string>
#include <vector>
#include "estacao.h"

using namespace std;

class SistemaMeteorologico {
public:
	bool insereEstacao (string);
	bool insereLeitura (string, string, double);

	void mostraRelatorio () const;                         // itens 1 e 2
	void mostraMediaMovel (string) const;                  // item 3
	void ordenaEstacoes (string) const;                    // item 4
	void previsaoProximaLeitura (string, string) const;    // item 5

	void geraDadosAleatorios (int = 7);
	int numeroEstacoes () const;

	static const double LIMITE_VARIACAO;

private:
	vector <Estacao> estacoes;
	int buscaEstacao (string) const;
};
