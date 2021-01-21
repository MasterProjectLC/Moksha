#include "Jenna.h"

Jenna::Jenna(Mapa* m) : NPC(m) {}

void Jenna::takeAction() {
	if (salaAtual->getName() != salaAlvo->getName() && !caminho.empty()) {
		move(*caminho.front());
		caminho.pop();
	}
}