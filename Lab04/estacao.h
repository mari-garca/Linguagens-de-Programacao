#include <iostream>
#include <string>
#include <vector>
#include "leiturasensor.h"

using namespace std;

class Estacao {
public:
	Estacao (string = "SEM-NOME");

	void setNome (string);
	string getNome () const;

	void insereLeitura (string, double);

	bool possuiGrandeza (string) const;
	int numeroLeituras (string) const;  
	int ultimoInstante (string) const;  

	vector <double> obtemValores (string) const;

	vector <double> calculaMediaMovel (string) const;

	bool regressaoLinear (string, double &, double &) const;

	void mostraLeituras () const;

	static const int JANELA = 3;

private:
	string nome;
	vector <LeituraSensor> leituras;

	vector <string> grandezas;
	vector <int> contadores;

	int indiceGrandeza (string) const;
	vector <LeituraSensor> filtraLeituras (string) const;
	void ordenaPorInstante (vector <LeituraSensor> &) const;
};
