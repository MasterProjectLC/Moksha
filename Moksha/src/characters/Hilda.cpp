#include "Hilda.h"

Hilda::Hilda(Map* m) : NPC{ m, "Hilda", "B U F F L A D Y", F, 2, 1 } {
	trackablePeople.insert("Santos");
}

void Hilda::setupActionsParticular() {

}


void Hilda::setupWorldParticular() {
	goap_worldstate_set(&ap, &world, "with_Santos", true);
}


void Hilda::setupObjectivesParticular() {

}


int Hilda::decideActionParticular(string acao) {
	return descansar;
}


void Hilda::advancePlansExtra(string currentProcess) {

}


void Hilda::updateWorldExtra() {
	// describe current world state.
}