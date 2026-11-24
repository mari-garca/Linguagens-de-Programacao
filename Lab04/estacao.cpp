#include <iomanip>
#include "estacao.h"

Estacao::Estacao (string n) {
	setNome (n);
}

void Estacao::setNome (string n) {
	if (n.length () == 0) {
		cout << "[!] Nome vazio, a estacao sera chamada SEM-NOME" << endl;
		n = "SEM-NOME";
	}
	nome = n;
}

string Estacao::getNome () const {
	return nome;
}


int Estacao::indiceGrandeza (string g) const {
	for (size_t i = 0; i < grandezas.size (); i++)
		if (grandezas [i] == g)
			return i;
	return -1;
}

void Estacao::insereLeitura (string g, double v) {
	int i = indiceGrandeza (g);

	if (i == -1) {
		grandezas.push_back (g);
		contadores.push_back (0);
		i = grandezas.size () - 1;
	}

	contadores [i]++;

	LeituraSensor leitura (g, v, contadores [i]);
	leituras.push_back (leitura);
}

bool Estacao::possuiGrandeza (string g) const {
	return indiceGrandeza (g) != -1;
}

int Estacao::numeroLeituras (string g) const {
	return filtraLeituras (g).size ();
}

int Estacao::ultimoInstante (string g) const {
	int i = indiceGrandeza (g);
	if (i == -1)
		return 0;
	return contadores [i];
}

vector <LeituraSensor> Estacao::filtraLeituras (string g) const {
	vector <LeituraSensor> filtradas;

	for (size_t i = 0; i < leituras.size (); i++)
		if (leituras [i].getNome () == g)
			filtradas.push_back (leituras [i]);

	ordenaPorInstante (filtradas);
	return filtradas;
}

void Estacao::ordenaPorInstante (vector <LeituraSensor> &v) const {
	for (size_t prox = 1; prox < v.size (); prox++) {
		LeituraSensor insere = v [prox];
		int indice = prox;

		while ((indice > 0) &&
		       (v [indice - 1].getInstante () > insere.getInstante ())) {
			v [indice] = v [indice - 1];
			indice--;
		}
		v [indice] = insere;
	}
}

vector <double> Estacao::obtemValores (string g) const {
	vector <double> valores;

	vector <LeituraSensor> serie = filtraLeituras (g);

	for (size_t i = 0; i < serie.size (); i++)
		valores.push_back (serie [i].getValor ());

	return valores;
}


vector <double> Estacao::calculaMediaMovel (string g) const {
	vector <double> valores = obtemValores (g);
	vector <double> medias;
	int n = valores.size ();

	for (int a = JANELA - 1; a < n; a++) {
		double soma = 0.0;
		for (int i = 0; i < JANELA; i++)
			soma += valores [a - i];
		medias.push_back (soma / JANELA);
	}

	return medias;
}


bool Estacao::regressaoLinear (string g, double &a, double &b) const {
	vector <LeituraSensor> filtradas = filtraLeituras (g);
	int n = filtradas.size ();

	if (n < 2)
		return false;

	double somaX = 0.0, somaY = 0.0, somaXY = 0.0, somaXX = 0.0;

	for (int i = 0; i < n; i++) {
		double x = static_cast <double> (filtradas [i].getInstante ());
		double y = filtradas [i].getValor ();
		somaX += x;
		somaY += y;
		somaXY += x * y;
		somaXX += x * x;
	}

	double denominador = n * somaXX - somaX * somaX;
	if (denominador == 0.0)
		return false;

	a = (n * somaXY - somaX * somaY) / denominador;
	b = (somaY / n) - a * (somaX / n);
	return true;
}

void Estacao::mostraLeituras () const {
	cout << "> " << nome << endl;

	if (leituras.size () == 0) {
		cout << "    [sem leituras]" << endl;
		return;
	}

	cout << fixed << setprecision (1);

	for (size_t j = 0; j < grandezas.size (); j++) {
		string g = grandezas [j];
		vector <LeituraSensor> serie = filtraLeituras (g);

		cout << "  * " << g << " (" << serie.size () << " leituras)" << endl;

		cout << "      instante:";
		for (size_t i = 0; i < serie.size (); i++)
			cout << setw (7) << serie [i].getInstante ();
		cout << endl;

		cout << "      valor   :";
		for (size_t i = 0; i < serie.size (); i++)
			cout << setw (7) << serie [i].getValor ();
		cout << endl;
	}
}

