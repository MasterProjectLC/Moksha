#include "npc.h"

NPC::NPC(Mapa* m, string name, int genero, int forca, int destreza) : Personagem(genero, forca, destreza) {
	this->nome = name;
	this->mapa = m;

	FileDict fileObjeto = FileManager::readFromFile("files/personagens/" + getNome() + ".txt");
	this->dict = fileObjeto;
}


void NPC::executarReacao(string topico, string frase, string remetente) {
	if (isInconsciente())
		return;

	say(topico, dict.getValue(topico), vector<string>(1, remetente));
}