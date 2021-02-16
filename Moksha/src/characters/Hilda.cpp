#include "Hilda.h"

Hilda::Hilda(Mapa* m) : NPC{ m, "Hilda", F, 2, 1 } {
}

void Hilda::setupAcoesAdicional() {

}


void Hilda::setupMundoAdicional() {

}


void Hilda::setupObjetivosAdicional() {
	goap_worldstate_set(&ap, &world, "with_Baxter", true);
	goap_worldstate_set(&ap, &currentGoal.goal, "alive", true);
}


void Hilda::updatePlanosAdicional() {
	// describe current world state.

	planCost = astar_plan(&ap, world, currentGoal.goal, plan, states, &plansz);
	currentStep = -1;
	avancarPlanos();
}


int Hilda::decidirAcaoAdicional(string acao) {
	return descansar;
}


void Hilda::avancarPlanosAdicional() {

}