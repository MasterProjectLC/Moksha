#include "Hilda.h"

Hilda::Hilda(Mapa* m) : NPC{ m, "Hilda", F, 2, 1 } {
	trackablePeople.insert("Santos");
}

void Hilda::setupAcoesAdicional() {

}


void Hilda::setupMundoAdicional() {
	goap_worldstate_set(&ap, &world, "with_Santos", true);
}


void Hilda::setupObjetivosAdicional() {

}


int Hilda::decidirAcaoAdicional(string acao) {
	return descansar;
}


void Hilda::advancePlansExtra(string currentProcess) {

}


void Hilda::updateWorldExtra() {
	// describe current world state.
}