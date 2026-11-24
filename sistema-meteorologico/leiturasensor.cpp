#include "leiturasensor.h"

LeituraSensor::LeituraSensor (string n, double v, int i) {
	setNome (n);
	setValor (v);
	setInstante (i);
}

void LeituraSensor::setNome (string n) {
	if (n.length () == 0) {
		cout << "[!] Grandeza sem nome, usando \"indefinida\"" << endl;
		n = "indefinida";
	}
	nome = n;
}

string LeituraSensor::getNome () const {
	return nome;
}

void LeituraSensor::setValor (double v) {
	valor = v;
}

double LeituraSensor::getValor () const {
	return valor;
}

void LeituraSensor::setInstante (int i) {
	//correcao se o instante for negtivo
	if (i < 0) {
		cout << "[!] Instante negativo ajustado para 0" << endl;
		i = 0;
	}
	instante = i;
}

int LeituraSensor::getInstante () const {
	return instante;
}
