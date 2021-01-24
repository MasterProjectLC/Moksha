#pragma once
#include "personagem.h"
#include "mapa.h"
#include "fileManager.h"

using namespace std;

class NPC : public Personagem {
protected:
	Sala *salaAlvo;
	queue<Sala*> caminho;
	Mapa* mapa;

	vector<string> topicos;
	vector<vector<string>> reacoes;

	void descobrirCaminho() {
		caminho = mapa->optimalPath(salaAtual, salaAlvo);
	};

public:
	NPC() {};
	NPC(Mapa* m, string nome);

	void executarReacao(string topico, string remetente);

	void setSalaAlvo(Sala* nova) { salaAlvo = nova; descobrirCaminho(); }
};