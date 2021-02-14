#include "Baxter.h"

Baxter::Baxter(Mapa* m) : NPC{ m, "Baxter", M, 1, 1 } {
}

void Baxter::setupAcoesAdicional() {
	
}


void Baxter::setupMundoAdicional() {

}


void Baxter::setupObjetivosAdicional() {
	goap_worldstate_set(&ap, &current, "with_Hilda", true);
	goap_worldstate_set(&ap, &goal, "alive", true);
}


void Baxter::updatePlanosAdicional() {
	// describe current world state.

	planCost = astar_plan(&ap, current, goal, plan, states, &plansz);
	currentStep = -1;
	avancarPlanos();
}


void Baxter::tomarAcaoParticular(string acao) {
	
}


void Baxter::avancarPlanosAdicional() {
	
}