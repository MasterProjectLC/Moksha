#include "Willow.h"

Willow::Willow(Map* m) : NPC(m, "Willow") {

}

void Willow::setupActionsParticular() {
	addTrackablePeople("Baxter");
	addTrackablePeople("Liz", vector<string>({ "LizRoom", "GameRoom", "DiningHall" }));
	addTrackableRoom("BlakewellRoom");

	addTrackableConvo("willow_liz", "with_Liz");
	addTrackableNull("write", "writing", "BlakewellRoom");
}


void Willow::setupWorldParticular() {

}


void Willow::setupObjectivesParticular() {
	goap_worldstate_set(&ap, &currentGoal.goal, "writing", true);
}

void Willow::updateWorldExtra() {
	// describe current world state.
	
}

void Willow::setupProcessParticular(string currentProcess) {

}

int Willow::decideActionParticular(string action) {
	return descansar;
}