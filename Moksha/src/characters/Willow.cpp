#include "Willow.h"

Willow::Willow(Map* m) : NPC(m, "Willow") {

}

void Willow::setupActionsParticular() {
	addTrackablePeople("Baxter");
	addTrackableRoom("BlakewellRoom");
	goap_set_pre(&ap, "write", "in_BlakewellRoom", true);
	goap_set_pst(&ap, "write", "writing", true);
}


void Willow::setupWorldParticular() {
	goap_worldstate_set(&ap, &world, "in_BlakewellRoom", true);
	goap_worldstate_set(&ap, &world, "writing", false);
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
	if (action == "write") {
		actionArgs.push_back("writing.");
		return acaoNula;
	}

	return descansar;
}