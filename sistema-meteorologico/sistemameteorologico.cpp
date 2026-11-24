#include <iomanip>
#include <cstdlib>
#include "sistemameteorologico.h"

const double SistemaMeteorologico::LIMITE_VARIACAO = 15.0;

int SistemaMeteorologico::buscaEstacao (string nome) const {
	for (size_t i = 0; i < estacoes.size (); i++)
		if (estacoes [i].getNome () == nome)
			return i;
	return -1;
}

int SistemaMeteorologico::numeroEstacoes () const {
	return estacoes.size ();
}

bool SistemaMeteorologico::insereEstacao (string nome) {
	if (buscaEstacao (nome) != -1) {
		cout << "[!] Ja existe uma estacao chamada " << nome << endl;
		return false;
	}

	Estacao nova (nome);
	estacoes.push_back (nova);
	return true;
}

bool SistemaMeteorologico::insereLeitura (string nomeEstacao,
                                          string grandeza, double valor) {
	int i = buscaEstacao (nomeEstacao);

	if (i == -1) {
		cout << "[!] Estacao " << nomeEstacao << " nao encontrada" << endl;
		return false;
	}

	estacoes [i].insereLeitura (grandeza, valor);
	return true;
}


void SistemaMeteorologico::mostraRelatorio () const {
	cout << "\n### RELATORIO GERAL (" << estacoes.size ()
	     << " estacoes) ###" << endl;

	if (estacoes.size () == 0)
		cout << "  nenhuma estacao cadastrada ainda" << endl;

	for (size_t e = 0; e < estacoes.size (); e++) {
		Estacao estacao = estacoes [e];
		cout << endl;
		estacao.mostraLeituras ();
	}
}


void SistemaMeteorologico::mostraMediaMovel (string grandeza) const {
	int N = Estacao::JANELA;
	int contador = 0;

	cout << "\n### MEDIA MOVEL DE " << grandeza << " (janela de " << N
	     << " leituras) ###" << endl;
	cout << fixed << setprecision (2);

	for (size_t e = 0; e < estacoes.size (); e++) {
		Estacao estacao = estacoes [e];
		
		if (!estacao.possuiGrandeza (grandeza))
			continue;

		if (estacao.numeroLeituras (grandeza) < N) {
			cout << "\n> " << estacao.getNome () << ": [!] precisa de pelo menos "
			     << N << " leituras" << endl;
			continue;
		}

		vector <double> medias = estacao.calculaMediaMovel (grandeza);

		cout << "\n> " << estacao.getNome () << endl;

		
		cout << "      ate o instante:";
		for (size_t k = 0; k < medias.size (); k++)
			cout << setw (8) << k + N;
		cout << endl;

		cout << "      media movel   :";
		for (size_t k = 0; k < medias.size (); k++)
			cout << setw (8) << medias [k];
		cout << endl;

		contador++;
	}

	if (contador == 0)
		cout << "  nenhuma estacao com leituras suficientes de "
		     << grandeza << endl;
	else if (contador < 3)
		cout << "\n[!] So " << contador << " estacao(oes) possuem " << grandeza
		     << "; o ideal sao pelo menos 3" << endl;
}


void SistemaMeteorologico::ordenaEstacoes (string grandeza) const {
	int N = Estacao::JANELA;

	
	vector <string> nomes;
	vector <double> ultima;     // media movel da ultima janela
	vector <double> anterior;   // media movel da janela imediatamente anterior

	for (size_t e = 0; e < estacoes.size (); e++) {
		Estacao estacao = estacoes [e];
		// Estacoes sem a grandeza escolhida sao desconsideradas
		if (!estacao.possuiGrandeza (grandeza))
			continue;

		// Sao necessarias N + 1 leituras para existirem duas janelas
		if (estacao.numeroLeituras (grandeza) < N + 1) {
			cout << "[!] " << estacao.getNome () << " ficou de fora: menos de "
			     << N + 1 << " leituras de " << grandeza << endl;
			continue;
		}

		vector <double> medias = estacao.calculaMediaMovel (grandeza);
		nomes.push_back (estacao.getNome ());
		ultima.push_back (medias [medias.size () - 1]);
		anterior.push_back (medias [medias.size () - 2]);
	}

	if (nomes.size () == 0) {
		cout << "  nenhuma estacao com leituras suficientes de "
		     << grandeza << endl;
		return;
	}


	for (size_t prox = 1; prox < ultima.size (); prox++) {
		string insereNome = nomes [prox];
		double insereUltima = ultima [prox];
		double insereAnterior = anterior [prox];
		int indice = prox;

		while ((indice > 0) && (ultima [indice - 1] < insereUltima)) {
			nomes [indice] = nomes [indice - 1];
			ultima [indice] = ultima [indice - 1];
			anterior [indice] = anterior [indice - 1];
			indice--;
		}
		nomes [indice] = insereNome;
		ultima [indice] = insereUltima;
		anterior [indice] = insereAnterior;
	}

	cout << "\n### RANKING POR " << grandeza << " (maior media movel primeiro) ###"
	     << endl;
	cout << fixed << setprecision (2);

	for (size_t i = 0; i < nomes.size (); i++) {
		cout << "\n " << i + 1 << "o lugar: " << nomes [i] << endl;
		cout << "      media atual " << ultima [i]
		     << " | media anterior " << anterior [i] << endl;

		if (anterior [i] == 0.0) {
			cout << "      variacao: nao calculada (media anterior igual a zero)"
			     << endl;
			continue;
		}

		double variacao = (ultima [i] - anterior [i]) / anterior [i] * 100.0;

		cout << "      variacao: ";
		if (variacao >= 0)
			cout << "+";
		cout << variacao << "%";

		if (variacao > LIMITE_VARIACAO || variacao < -LIMITE_VARIACAO)
			cout << "   <<< ALERTA: variacao anormal" << endl;
		else
			cout << "   (dentro do esperado)" << endl;
	}

	cout << "\n  * alerta quando a variacao passa de " << LIMITE_VARIACAO
	     << "% para mais ou para menos" << endl;
}

void SistemaMeteorologico::previsaoProximaLeitura (string nomeEstacao,
                                                   string grandeza) const {
	int i = buscaEstacao (nomeEstacao);

	if (i == -1) {
		cout << "[!] Estacao " << nomeEstacao << " nao encontrada" << endl;
		return;
	}

	if (!estacoes [i].possuiGrandeza (grandeza)) {
		cout << "[!] " << nomeEstacao << " nao tem leituras de "
		     << grandeza << endl;
		return;
	}

	double a, b;
	if (!estacoes [i].regressaoLinear (grandeza, a, b)) {
		cout << "[!] A regressao precisa de pelo menos 2 leituras de "
		     << grandeza << endl;
		return;
	}

	int proximo = estacoes [i].ultimoInstante (grandeza) + 1;

	cout << "\n### PREVISAO: " << grandeza << " em " << nomeEstacao << " ###"
	     << endl;
	cout << fixed << setprecision (4);

	cout << "  reta ajustada: y = " << a << "x";
	if (b < 0)
		cout << " - " << -b << endl;
	else
		cout << " + " << b << endl;

	cout << "  a (coeficiente angular) = " << a << endl;
	cout << "  b (coeficiente linear)  = " << b << endl;
	cout << "  proxima leitura (instante " << proximo << ") ~ "
	     << setprecision (2) << a * proximo + b << endl;
}


void SistemaMeteorologico::geraDadosAleatorios (int numLeituras) {
	string nomes [3] = {"EST-PRAIA", "EST-SERRA", "EST-PORTO"};
	double baseTemp [3] = {28.0, 18.0, 24.0};
	double baseUmid [3] = {75.0, 85.0, 65.0};

	for (int e = 0; e < 3; e++) {
		if (buscaEstacao (nomes [e]) == -1)
			insereEstacao (nomes [e]);

		for (int t = 0; t < numLeituras; t++) {
			double temp = baseTemp [e] + (rand () % 41 - 20) / 10.0;

			double umid = baseUmid [e] + (rand () % 101 - 50) / 10.0;

			insereLeitura (nomes [e], "temperatura", temp);
			insereLeitura (nomes [e], "umidade", umid);
		}
	}
}
