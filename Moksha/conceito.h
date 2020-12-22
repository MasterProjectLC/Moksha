#pragma once
#include <string>

using namespace std;

class Conceito {
protected:
	string nome;
public:
	Conceito() {};
	Conceito(string nome);

	string getNome() { return nome; };
};
