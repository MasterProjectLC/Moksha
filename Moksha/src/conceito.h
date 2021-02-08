#pragma once
#include <string>

using namespace std;

class Conceito {
protected:
	string nome;
	string descricao;
public:
	Conceito() {};
	Conceito(string nome);
	Conceito(string nome, string descricao);

	string getNome() { return nome; };
	string getDescricao() { return descricao; };
};
