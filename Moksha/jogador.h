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
	string erroSemItem;
	string erroMente;
	vector<string> acoesBasicas{ "obter", "mover", "examinar", "ver", "tocar", "mencionar" };

	map<string, set<string>> mindTheory;

	void mencionar(string topic, string person);
	void mover(string location);
	void interagir(string acao, string objeto);

	void addToMind(string topic, string character);
	bool isAcaoValida(string acao);

public:
	Jogador();
	void receberArgs(vector<string> args);

	void verPessoaEntrando(string nomePessoa);
	void executarReacao(string topico, string frase, string remetente);
	void verSala(vector<Personagem*> pessoasNaSala);

	void serAtacado(Personagem* atacante) {
		printText(atacante->getNome() + " te atacou!");
		Personagem::serAtacado(atacante);
	};
};