#include "personagem.h"

void Personagem::printText(string str) {
	notifyText = str;
	notify(imprimir);
}

void Personagem::move(string str) {
	notifyText = str;
	notify(mover);
}

void Personagem::move(Sala sala) {
	move(sala.getNome());
}

void Personagem::mention(string obj, set<string> receivers) {
	if (inventario.temConceito(obj) || inventario.temItem(obj)) {
		notifyText = obj;
		notifyTargets = receivers;
		notify(mencionar);
	}
	else {
		// TODO: deixar isso exclusivo do Jogador, e integrar com o sistema de arquivo
		printText("Hm? Nao sei o que e isso...");
	}
}

void Personagem::attack(string target) {
	notifyText = target;
	notify(atacar);
}

void Personagem::say(string topico, string str, set<string> receivers) {
	notifyText = topico + "|" + str;
	notifyTargets = receivers;
	notify(falar);
}

void Personagem::rest() {
	notify(descansar);
}

void Personagem::talk(string convo, set<string> participants) {
	notifyText = convo;
	notifyTargets = participants;
	notify(conversar);
}