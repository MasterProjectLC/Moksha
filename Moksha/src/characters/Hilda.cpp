#include "Hilda.h"

Hilda::Hilda(Mapa* m) : NPC{ m, "Hilda", F, 2, 1 } {
}

void Hilda::setupAcoesAdicional() {

}


void Hilda::setupMundoAdicional() {

}


void Hilda::setupObjetivosAdicional() {
	goap_worldstate_set(&ap, &current, "with_Baxter", true);
	goap_worldstate_set(&ap, &goal, "alive", true);
}


void Hilda::updatePlanosAdicional() {
	// describe current world state.

	planCost = astar_plan(&ap, current, goal, plan, states, &plansz);
	currentStep = -1;
	avancarPlanos();
}


void Hilda::tomarAcaoParticular(string acao) {

}


void Hilda::avancarPlanosAdicional() {

}