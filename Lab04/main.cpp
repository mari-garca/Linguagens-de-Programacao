#include <iostream>
#include <string>
#include <cstdlib>
#include <ctime>
#include "sistemameteorologico.h"

using namespace std;

enum Opcoes { SAIR, INSERE_ESTACAO, INSERE_LEITURA, MEDIA_MOVEL,
              ORDENACAO, PREVISAO, DADOS_PRONTOS, RELATORIO };

void mostraMenu ();
int leInteiro ();
double leDouble ();
string leTexto (string);

int main () {
	SistemaMeteorologico sistema;
	string grandezaEscolhida = "";  
	string nome, grandeza;
	double valor;
	int opcao;

	srand (time (0)); 

	do {
		mostraMenu ();
		opcao = leInteiro ();

		switch (opcao) {
		case INSERE_ESTACAO:
			nome = leTexto ("  nome da estacao (ex.: EST-CENTRO) > ");
			if (sistema.insereEstacao (nome))
				cout << "[ok] " << nome << " cadastrada" << endl;
			sistema.mostraRelatorio ();
			break;

		case INSERE_LEITURA:
			nome = leTexto ("  estacao > ");
			grandeza = leTexto ("  grandeza (ex.: temperatura) > ");
			cout << "  valor > ";
			valor = leDouble ();
			if (sistema.insereLeitura (nome, grandeza, valor))
				cout << "[ok] leitura registrada" << endl;
			sistema.mostraRelatorio ();
			break;

		case MEDIA_MOVEL:
			grandezaEscolhida = leTexto ("  grandeza > ");
			sistema.mostraMediaMovel (grandezaEscolhida);
			break;

		case ORDENACAO:
			if (grandezaEscolhida == "") {
				cout << "[!] Nenhuma grandeza foi escolhida na opcao 3 ainda" << endl;
				grandezaEscolhida = leTexto ("  grandeza > ");
			}
			else
				cout << "  usando a grandeza da opcao 3: "
				     << grandezaEscolhida << endl;
			sistema.ordenaEstacoes (grandezaEscolhida);
			break;

		case PREVISAO:
			nome = leTexto ("  estacao > ");
			grandeza = leTexto ("  grandeza > ");
			sistema.previsaoProximaLeitura (nome, grandeza);
			break;

		case DADOS_PRONTOS:
			// Estacoes predef com valores rand
			sistema.geraDadosAleatorios ();
			cout << "[ok] estacoes pre-definidas carregadas" << endl;
			sistema.mostraRelatorio ();
			break;

		case RELATORIO:
			sistema.mostraRelatorio ();
			break;

		case SAIR:
			cout << "Adios!" << endl;
			break;

		default:
			cout << "[!] Opcao invalida, tente de novo" << endl;
		}
	} while (opcao != SAIR);

	return 0;
}

void mostraMenu () {
	cout << "\n------------------------------------------" << endl
	     << "   REDE DE ESTACOES METEOROLOGICAS" << endl
	     << "------------------------------------------" << endl
	     << " [" << INSERE_ESTACAO   << "] Cadastrar estacao" << endl
	     << " [" << INSERE_LEITURA   << "] Registrar leitura" << endl
	     << " [" << MEDIA_MOVEL      << "] Media movel de uma grandeza" << endl
	     << " [" << ORDENACAO        << "] Ranking e alerta de variacao" << endl
	     << " [" << PREVISAO         << "] Previsao (regressao linear)" << endl
	     << " [" << DADOS_PRONTOS    << "] Carregar estacoes pre-definidas" << endl
	     << " [" << RELATORIO        << "] Ver relatorio" << endl
	     << " [" << SAIR             << "] Sair" << endl
	     << ">> escolha: ";
}

int leInteiro () {
	int n;
	cin >> n;
	if (!cin) {
		cin.clear ();   // limpa o estado de erro do cin
		n = -1;
	}
	cin.ignore (10000, '\n');   // descarta o restante da linha
	return n;
}


double leDouble () {
	double v;
	cin >> v;
	while (!cin) {
		cin.clear ();
		cin.ignore (10000, '\n');
		cout << "  [!] numero invalido, digite de novo > ";
		cin >> v;
	}
	cin.ignore (10000, '\n');
	return v;
}


string leTexto (string mensagem) {
	string texto;
	do {
		cout << mensagem;
		getline (cin, texto);
	} while (texto.length () == 0 && cin);
	return texto;
}
