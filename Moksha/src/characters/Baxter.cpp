#include "Baxter.h"

Baxter::Baxter(Map* m) : NPC{ m, "Baxter", "Posh bitch", M, 1, 1 } {
	trackablePeople.insert("Santos");
}

void Baxter::setupActionsParticular() {
	
}


void Baxter::setupWorldParticular() {
	goap_worldstate_set(&ap, &world, "with_Santos", true);
}


void Baxter::setupObjectivesParticular() {
	goap_worldstate_set(&ap, &currentGoal.goal, "with_Santos", true);
}


int Baxter::decideActionParticular(string action) {
	return descansar;
}


void Baxter::advancePlansExtra(string currentProcess) {

}


void Baxter::updateWorldExtra() {
	// describe current world state.
}