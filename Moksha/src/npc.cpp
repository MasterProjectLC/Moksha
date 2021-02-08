#include "npc.h"

NPC::NPC(Mapa* m, string name, int genero, int forca, int destreza) : Personagem(genero, forca, destreza) {
	this->nome = name;
	this->mapa = m;
	this->alvo = alvo;

	FileDict fileObjeto = FileManager::readFromFile("files/personagens/" + getNome() + ".txt");
	this->dict = fileObjeto;
}


// PATHFINDING ---------------

queue<Sala*> NPC::descobrirCaminho(Sala* salaAlvo) {
	return descobrirCaminho(salaAtual, salaAlvo);
};

queue<Sala*> NPC::descobrirCaminho(Sala* salaInicial, Sala* salaAlvo) {
	return mapa->optimalPath(salaInicial, salaAlvo);
};

queue<Sala*> NPC::procurar() {
	return procurar(salaAtual);
};

queue<Sala*> NPC::procurar(Sala* salaPista) {
	queue<Sala*> retorno;
	retorno.push(salaAtual);
	if (salaPista != salaAtual)
		retorno = descobrirCaminho(salaPista);
	queue<Sala*> procura = mapa->breadthSearch(salaPista);
	while (!procura.empty()) {
		queue<Sala*> caminho = descobrirCaminho(retorno.back(), procura.front());
		while (!caminho.empty()) {
			retorno.push(caminho.front());
			caminho.pop();
		}
		procura.pop();
	}

	retorno.pop();
	return retorno;
};


void NPC::seguirCaminho() {
	if (!caminho.empty()) {
		move(*caminho.front());
		caminho.pop();
	}
}


// ----------------------------------

void NPC::executarReacao(string topico, string frase, string remetente) {
	if (isInconsciente())
		return;

	say(topico, dict.getValue(topico), vector<string>(1, remetente));
}


void NPC::verSala(vector<Personagem*> pessoasNaSala) {
	goap_worldstate_set(&ap, &current, "com_jogador", false);

	for (int i = 0; i < pessoasNaSala.size(); i++)
		if (pessoasNaSala[i]->getNome() == alvo) {
			goap_worldstate_set(&ap, &current, "com_jogador", true);
			updatePlanos();
			break;
		}
};


void NPC::verPessoaMovendo(Personagem* pessoa, string outraSala, bool entrando) {
	if (pessoa->getNome() == alvo) {
		if (!entrando) {
			string alvoVec[1] = { alvo };
			ultimoAvistamento.addPair(set<string>(alvoVec, alvoVec+1), outraSala);
		}

		goap_worldstate_set(&ap, &current, "com_jogador", entrando);
		updatePlanos();
	}
}