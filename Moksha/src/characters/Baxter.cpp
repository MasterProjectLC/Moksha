#include "Baxter.h"

Baxter::Baxter(Mapa* m) : NPC{ m, "Baxter", "Posh bitch", M, 1, 1 } {
	trackablePeople.insert("Santos");
}

void Baxter::setupAcoesAdicional() {
	
}


void Baxter::setupMundoAdicional() {
	goap_worldstate_set(&ap, &world, "with_Santos", true);
}


void Baxter::setupObjetivosAdicional() {
	goap_worldstate_set(&ap, &currentGoal.goal, "with_Santos", true);
}


int Baxter::decidirAcaoAdicional(string acao) {
	return descansar;
}


void Baxter::advancePlansExtra(string currentProcess) {

}


void Baxter::updateWorldExtra() {
	// describe current world state.
}