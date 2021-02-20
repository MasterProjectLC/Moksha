#include "Hilda.h"

Hilda::Hilda(Mapa* m) : NPC{ m, "Hilda", F, 2, 1 } {
}

void Hilda::setupAcoesAdicional() {

}


void Hilda::setupMundoAdicional() {
	goap_worldstate_set(&ap, &world, "with_Baxter", true);
}


void Hilda::setupObjetivosAdicional() {
	goap_worldstate_set(&ap, &currentGoal.goal, "with_Baxter", true);
	goap_worldstate_set(&ap, &currentGoal.goal, "Hilda_alive", true);
}


int Hilda::decidirAcaoAdicional(string acao) {
	return descansar;
}


void Hilda::advancePlansExtra(string currentProcess) {

}


void Hilda::updateWorldExtra() {
	// describe current world state.
}