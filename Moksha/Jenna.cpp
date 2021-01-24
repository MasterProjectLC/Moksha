#include "Jenna.h"

Jenna::Jenna(Mapa* m) : NPC{m, "Jenna"} {}

void Jenna::takeAction() {
	if (salaAtual->getName() != salaAlvo->getName() && !caminho.empty()) {
		move(*caminho.front());
		caminho.pop();
	}
}