#include "personagem.h"

void Personagem::setSalaAtual(Sala *sala) { 
	salaAtual = sala;
}

Sala* Personagem::getSalaAtual() {
	return salaAtual; 
}

void Personagem::printText(string str) {
	notifyText = str;
	notify(imprimir);
}

void Personagem::move(string str) {
	notifyText = str;
	notify(mover);
}

void Personagem::move(Sala sala) {
	move(sala.getName());
}

void Personagem::mention(string obj, vector<string> receivers) {
	notifyText = obj;
	notifyTargets = receivers;
	notify(mencionar);
}

void Personagem::say(string topico, string str, vector<string> receivers) {
	notifyText = topico + "|" + str;
	notifyTargets = receivers;
	notify(falar);
}