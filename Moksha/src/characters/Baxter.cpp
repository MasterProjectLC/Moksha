#include "Baxter.h"

Baxter::Baxter(Mapa* m) : NPC{ m, "Baxter", M, 1, 1 } {
}

void Baxter::setupAcoesAdicional() {
	
}


void Baxter::setupMundoAdicional() {

}


void Baxter::setupObjetivosAdicional() {
	goap_worldstate_set(&ap, &world, "with_Hilda", true);
	goap_worldstate_set(&ap, &currentGoal.goal, "alive", true);
}


void Baxter::updatePlanosAdicional() {
	// describe current world state.

	planCost = astar_plan(&ap, world, currentGoal.goal, plan, states, &plansz);
	currentStep = -1;
	avancarPlanos();
}


int Baxter::decidirAcaoAdicional(string acao) {
	return descansar;
}


void Baxter::avancarPlanosAdicional() {
	
}