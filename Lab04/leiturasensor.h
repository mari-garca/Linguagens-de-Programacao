//leiturasensor.h

#include <iostream>
#include <string>

using namespace std;

class LeituraSensor {
public:
	LeituraSensor (string = "indefinida", double = 0.0, int = 0);

	void setNome (string);
	string getNome () const;

	void setValor (double);
	double getValor () const;

	void setInstante (int);
	int getInstante () const;

private:
	string nome;    
	double valor;
	int instante;   
};
