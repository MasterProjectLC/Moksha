#include "npc.h"

NPC::NPC(Mapa* m, string name) { 
	this->nome = name;
	this->mapa = m;

	FileDict fileObjeto = FileManager::readFromFile("files/personagens/" + getNome() + ".txt");

	for (int i = 0; i < fileObjeto.getKeys().size(); i++) {
		topicos.push_back(fileObjeto.getKeys()[i][0]);
		reacoes.push_back(fileObjeto.getValues(fileObjeto.getKeys()[i][0]));
	}
}


void NPC::executarReacao(string topico, string remetente) {
	for (int i = 0; i < topicos.size(); i++) {
		if (topico == topicos[i]) {
			say(reacoes[i][0], vector<string>(1, remetente));
		}
	}
}