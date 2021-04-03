#include "Paul.h"

Paul::Paul(Map* m) : NPC{ m, "Paul", 
"Paul Davis is Renard's quiet assistant. While seemingly curious about the inner workings of the ship, he doesn't seem that interested in helping Renard.\n"
"Paul has a rough ginger beard and some untamed hair. Right now, he wears a closed leather jacket and a discreet newsy cap.", 
M, 7, 9 } {
	trackablePeople.insert("Santos");
}

void Paul::setupActionsParticular() {
	addTrackableRoom("RenardRoom");
	goap_set_pre(&ap, "plan", "in_RenardRoom", true);
	goap_set_pst(&ap, "plan", "planning", true);
}


void Paul::setupWorldParticular() {
	goap_worldstate_set(&ap, &world, "planning", false);
}


void Paul::setupObjectivesParticular() {
	goap_worldstate_set(&ap, &currentGoal.goal, "planning", true);
}


void Paul::updateWorldExtra() {
	// describe current world state.
}

void Paul::updateProcessExtra(string currentProcess) {

}

int Paul::decideActionParticular(string action) {
	if (action == "plan") {
		actionArgs.push_back("planning.");
		return acaoNula;
	}
	return descansar;
}