#include "Baxter.h"

Baxter::Baxter(Mapa* m) : NPC{ m, "Baxter", M, 1, 1 } {
}

void Baxter::setupAcoesAdicional() {
	
}


void Baxter::setupMundoAdicional() {

}


void Baxter::setupObjetivosAdicional() {
	goap_worldstate_set(&ap, &current, "com_Hilda", true);
	goap_worldstate_set(&ap, &goal, "vivo", true);
}


void Baxter::updatePlanos() {
	// describe current world state.

	planCost = astar_plan(&ap, current, goal, plan, states, &plansz);
	currentStep = -1;
	avancarPlanos();
}


void Baxter::tomarAcao() {
	
}


void Baxter::avancarPlanos() {
	
}