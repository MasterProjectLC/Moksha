#include "Baxter.h"

Baxter::Baxter(Mapa* m) : NPC{ m, "Baxter", M, 1, 1 } {
}

void Baxter::setupAcoesAdicional() {
	
}


void Baxter::setupMundoAdicional() {
	goap_worldstate_set(&ap, &world, "with_Hilda", true);
}


void Baxter::setupObjetivosAdicional() {
	goap_worldstate_set(&ap, &currentGoal.goal, "with_Hilda", true);
	goap_worldstate_set(&ap, &currentGoal.goal, "Baxter_alive", true);
}


int Baxter::decidirAcaoAdicional(string acao) {
	return descansar;
}


void Baxter::advancePlansExtra(string currentProcess) {

}


void Baxter::updateWorldExtra() {
	// describe current world state.
}