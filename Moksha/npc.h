#pragma once
#include "personagem.h"
#include "mapa.h"


class NPC : public Personagem {
protected:
	Sala *salaAlvo;
	queue<Sala*> caminho;
	Mapa* mapa;

	void descobrirCaminho() {
		caminho = mapa->optimalPath(salaAtual, salaAlvo);
	};

public:
	NPC() {};
	NPC(Mapa* m) { this->mapa = m; }

	void setSalaAlvo(Sala* nova) { salaAlvo = nova; descobrirCaminho(); }
};