#pragma once
#include "personagem.h"
#include "mapa.h"
#include "fileManager.h"
#include "dictionary.h"

using namespace std;

class NPC : public Personagem {
protected:
	Sala *salaAlvo;
	queue<Sala*> caminho;
	Mapa* mapa;

	Dictionary<vector<string>> dict;

	void descobrirCaminho() {
		caminho = mapa->optimalPath(salaAtual, salaAlvo);
	};

public:
	NPC() {};
	NPC(Mapa* m, string nome);

	void executarReacao(string topico, string frase, string remetente);

	void setSalaAlvo(Sala* nova) { salaAlvo = nova; descobrirCaminho(); }
};