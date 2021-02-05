#include "Jenna.h"

Jenna::Jenna(Mapa* m) : NPC{m, "Jenna", F, 1, 1} {}

void Jenna::takeAction() {
	if (salaAtual->getNome() != salaAlvo->getNome() && !caminho.empty()) {
		move(*caminho.front());
		caminho.pop();
	}
}