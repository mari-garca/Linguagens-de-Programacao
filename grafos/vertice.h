#ifndef VERTICE_H
#define VERTICE_H

#include <string>

using namespace std;

class Vertice {
public:
    Vertice(string = "");
    void setRotulo(string);
    string getRotulo();

private:
    string rotulo;
};

#endif
