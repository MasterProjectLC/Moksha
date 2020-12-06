#pragma once
#include <string>

using namespace std;

class Conceito {
private:
	string nome;
public:
	Conceito(string nome);

	string getNome() { return nome; };
};
