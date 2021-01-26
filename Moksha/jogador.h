#pragma once
#include <map>
#include <set>
#include "personagem.h"
#include "stringLib.h"
#include "fileManager.h"

using namespace std;

class Jogador : public Personagem {
private:
	string erroSemObjeto;
	string erroSemAcao;
	string erroSemSala;
	vector<string> acoesBasicas{ "obter", "mover", "examinar", "ver", "tocar", "mencionar" };

	map<string, set<string>> mindTheory;

public:
	Jogador();
	void receberArgs(vector<string> args);

	bool isAcaoValida(string acao);
	void verPessoaEntrando(string nomePessoa);
	void addToMind(string topic, string character);

	void executarReacao(string topico, string frase, string remetente);
	void verSala(vector<string> pessoasNaSala);
};